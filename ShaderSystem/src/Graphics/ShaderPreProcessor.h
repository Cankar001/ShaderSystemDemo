#pragma once

#include "Core/Assert.h"
#include "Core/Core.h"
#include "Core/Hash.h"
#include "ShaderBuffer.h"

#include <regex>
#include <string>
#include <filesystem>
#include <unordered_set>
#include <sstream>

#define SHADER_SYSTEM_SPECIAL_MACRO_NAME "__SHADER_SYSTEM_"

namespace ShaderSystem
{
	struct IncludeData
	{
		std::filesystem::path IncludedFilePath{};
		uint64_t IncludeDepth = 0;
		bool IsRelative = false;
		bool IsGuarded = false;
		uint64_t HashValue = 0;

		ShaderDomain IncludedType = ShaderDomain::None;

		bool operator==(const IncludeData& other) const
		{
			return IncludedFilePath == other.IncludedFilePath && HashValue == other.HashValue;
		}

		bool operator!=(const IncludeData& other) const
		{
			return !(*this == other);
		}
	};

	struct HeaderCache
	{
		std::string Source;
		uint64_t SourceHash;
		ShaderDomain Type;
		bool IsGuarded;
	};
}

namespace std
{
	template<>
	struct hash<ShaderSystem::IncludeData>
	{
		uint64_t operator()(const ShaderSystem::IncludeData& includeData) const noexcept
		{
			return ShaderSystem::HashString(includeData.IncludedFilePath.string()) ^ includeData.HashValue;
		}
	};
}

namespace ShaderSystem
{
	struct StageData
	{
		std::unordered_set<IncludeData> Headers;
		uint64_t HashValue = 0;

		bool operator==(const StageData& other) const
		{
			return Headers == other.Headers && HashValue == other.HashValue;
		}

		bool operator!=(const StageData& other) const
		{
			return !(*this == other);
		}
	};

	namespace utils
	{
		static std::vector<std::string> SplitStringsAndKeepDelimiters(std::string& str)
		{
			const static std::regex re(R"((^\W|^\w+)|(\w+)|[:()])", std::regex_constants::optimize);

			std::regex_iterator<std::string::iterator> rit(str.begin(), str.end(), re);
			std::regex_iterator<std::string::iterator> rend;
			std::vector<std::string> result;

			while (rit != rend)
			{
				result.emplace_back(rit->str());
				++rit;
			}

			return result;
		}

		template<bool RemoveHeaderGuard = false>
		bool ContainsHeaderGuard(std::string& header)
		{
			uint64_t pos = header.find('#');
			while (pos != std::string::npos)
			{
				const uint64_t endOfLine = header.find_first_of("\r\n", pos) + 1;
				auto tokens = utils::SplitStringsAndKeepDelimiters(header.substr(pos, endOfLine - pos));
				auto it = tokens.begin();

				if (*(++it) == "pragma")
				{
					if (*(++it) == "once")
					{
						if constexpr (RemoveHeaderGuard)
							header.erase(pos, endOfLine - pos);
						return true;
					}
				}
				pos = header.find('#', pos + 1);
			}
			return false;
		}

		// From https://wandbox.org/permlink/iXC7DWaU8Tk8jrf3
		enum class State : char
		{
			SlashOC,
			StarIC,
			SingleLineComment,
			MultiLineComment,
			NotAComment
		};

		template<typename InputIt, typename OutputIt>
		void CopyWithoutComments(InputIt first, InputIt last, OutputIt out)
		{
			State state = State::NotAComment;

			while (first != last)
			{
				switch (state)
				{
				case State::SlashOC:
				{
					if (*first == '/')
					{
						state = State::SingleLineComment;
					}
					else if (*first == '*')
					{
						state = State::MultiLineComment;
					}
					else
					{
						state = State::NotAComment;
						*out++ = '/';
						*out++ = *first;
					}

					break;
				}

				case State::StarIC:
				{
					if (*first == '/')
					{
						state = State::NotAComment;
					}
					else
					{
						state = State::MultiLineComment;
					}

					break;
				}

				case State::NotAComment:
				{
					if (*first == '/')
					{
						state = State::SlashOC;
					}
					else
					{
						*out++ = *first;
					}
					break;
				}

				case State::SingleLineComment:
				{
					if (*first == '\n')
					{
						state = State::NotAComment;
						*out++ = '\n';
					}
					break;
				}

				case State::MultiLineComment:
				{
					if (*first == '*')
					{
						state = State::StarIC;
					}
					else if (*first == '\n')
					{
						*out++ = '\n';
					}
					break;
				}
				}

				++first;
			}
		}

		static ShaderDomain ShaderDomainFromString(const std::string& str)
		{
			if (str == "vertex") return ShaderDomain::Vertex;
			if (str == "fragment") return ShaderDomain::Fragment;
			if (str == "compute") return ShaderDomain::Compute;
			if (str == "tess_control") return ShaderDomain::TessControl;
			if (str == "tess_eval") return ShaderDomain::TessEvalulation;
			if (str == "geometry") return ShaderDomain::Geometry;

			SHADER_SYSTEM_ASSERT(false);
			return ShaderDomain::None;
		}

		static std::string_view StageToShaderMacro(const std::string_view stage)
		{
			if (stage == "vertex") return "__VERTEX_STAGE__";
			if (stage == "fragment") return "__FRAGMENT_STAGE__";
			if (stage == "compute") return "__COMPUTE_STAGE__";
			if (stage == "geometry") return "__GEOMETRY_STAGE__";
			if (stage == "tesscontrol") return "__TESS_CONTROL_STAGE__";
			if (stage == "tesseval") return "__TESS_EVAL_STAGE__";

			SHADER_SYSTEM_ASSERT(false, "Unknown shader stage.");
			return "";
		}
	}

	class ShaderPreProcessor
	{
	public:

		template<ShaderLanguage Lang>
		static ShaderDomain PreprocessHeader(std::string& contents, bool& isGuarded, std::unordered_set<std::string>& specialMacros, const std::unordered_set<IncludeData>& includeData, const std::filesystem::path& fullPath);

		template<ShaderLanguage Lang>
		static std::unordered_map<ShaderDomain, std::string> PreprocessShader(const std::string& source, std::unordered_set<std::string>& specialMacros);
	};

	template<ShaderLanguage Lang>
	ShaderDomain ShaderPreProcessor::PreprocessHeader(std::string& contents, bool& isGuarded, std::unordered_set<std::string>& specialMacros, const std::unordered_set<IncludeData>& includeData, const std::filesystem::path& fullPath)
	{
		std::stringstream sourceStream;
		utils::CopyWithoutComments(contents.begin(), contents.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();

		ShaderDomain stagesInHeader = ShaderDomain::None;

		isGuarded = utils::ContainsHeaderGuard<true>(newSource);

		uint32_t stageCount = 0;
		uint64_t startOfShaderStage = newSource.find('#', 0);

		while (startOfShaderStage != std::string::npos)
		{
			const uint64_t endOfLine = newSource.find_first_of("\r\n", startOfShaderStage) + 1;
			auto& tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(startOfShaderStage, endOfLine - startOfShaderStage));

			uint32_t index = 0;
			if (tokens[index] == "#")
			{
				++index;

				if (index >= tokens.size())
				{
					SHADER_SYSTEM_WARN("Could not find any pre-processor directive!");
					break;
				}

				++index; // Skip the pragma directive
				if (tokens[index] == "shader")
				{
					SHADER_SYSTEM_ASSERT(tokens[index + 1] == ":");

					const std::string_view stage(tokens[index + 2]);
					SHADER_SYSTEM_ASSERT(stage == "vertex" || stage == "fragment" || stage == "pixel" || stage == "geometry" || stage == "compute" || stage == "tess_control" || stage == "tess_eval", "Invalid shader type");
					ShaderDomain foundStage = utils::ShaderDomainFromString(std::string(stage.begin(), stage.end()));

					const bool alreadyIncluded = std::find_if(includeData.begin(), includeData.end(), [fullPath](const IncludeData& data)
					{
						return fullPath == data.IncludedFilePath;
					}) != includeData.end();

					if (isGuarded && alreadyIncluded)
					{
						newSource.clear();
					}
					else if (!isGuarded && alreadyIncluded)
					{
						SHADER_SYSTEM_WARN("\"{0}\" Header does not contain a header guard (#pragma once).", fullPath.string().c_str());
					}

					if (stageCount == 0)
					{
						newSource.replace(startOfShaderStage, endOfLine - startOfShaderStage, fmt::format("#ifdef {}", utils::StageToShaderMacro(stage)));
					}
					else
					{
						newSource.replace(startOfShaderStage, endOfLine - startOfShaderStage, fmt::format("#endif\r#ifdef {}", utils::StageToShaderMacro(stage)));
					}

					*(int32_t*)&stagesInHeader |= (int32_t)foundStage;
					++stageCount;
				}
				else if (tokens[index] == "ifdef")
				{
					++index;
					if (tokens[index].rfind(SHADER_SYSTEM_SPECIAL_MACRO_NAME, 0) == 0)
					{
						specialMacros.emplace(tokens[index]);
					}
				}
				else if (tokens[index] == "if" || tokens[index] == "define")
				{
					++index;
					for (uint64_t i = 0; i < tokens.size(); ++i)
					{
						if (tokens[index].rfind(SHADER_SYSTEM_SPECIAL_MACRO_NAME, 0) == 0)
						{
							specialMacros.emplace(tokens[index]);
						}
					}
				}
			}

			startOfShaderStage = newSource.find('#', startOfShaderStage + 1);
		}

		if (stageCount)
		{
			newSource.append("\n#endif");
		}
		else
		{
			const bool alreadyIncluded = std::find_if(includeData.begin(), includeData.end(), [fullPath](const IncludeData& data)
			{
				return data.IncludedFilePath == fullPath;
			}) != includeData.end();

			if (isGuarded && alreadyIncluded)
			{
				newSource.clear();
			}
			else if (!isGuarded && alreadyIncluded)
			{
				SHADER_SYSTEM_WARN("{0} Header does not contain a header guard (#pragma once)", fullPath.string().c_str());
			}
		}

		contents = newSource;
		return stagesInHeader;
	}

	template<ShaderLanguage Lang>
	std::unordered_map<ShaderDomain, std::string> ShaderPreProcessor::PreprocessShader(const std::string& source, std::unordered_set<std::string>& specialMacros)
	{
		std::stringstream sourceStream;
		utils::CopyWithoutComments(source.begin(), source.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();

		std::unordered_map<ShaderDomain, std::string> shaderSources;
		std::vector<std::pair<ShaderDomain, uint64_t>> stagePositions;
		SHADER_SYSTEM_ASSERT(newSource.size(), "Shader is empty!");

		uint64_t startOfStage = 0;
		uint64_t pos = newSource.find('#');
		if (pos == std::string::npos)
		{
			SHADER_SYSTEM_FATAL("Could not find any '#' symbol. Aborting.");
			return shaderSources;
		}

		// Check first #version
		if (Lang == ShaderLanguage::GLSL)
		{
			const uint64_t endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			const std::vector<std::string>& tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(pos, endOfLine - pos));
			SHADER_SYSTEM_ASSERT(tokens.size() >= 3 && tokens[1] == "version", "Invalid #version encountered or #version is NOT encounted first.");
			pos = newSource.find('#', pos + 1);
		}

		while (pos != std::string::npos)
		{
			const uint64_t endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			std::vector<std::string>& tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(pos, endOfLine - pos));

			uint64_t index = 1; // Skip #
			if (tokens[index] == "pragma") // Parse stage. example: #pragma stage : vert
			{
				++index;
				if (tokens[index] == "shader")
				{
					++index;
					// Jump over ':'
					SHADER_SYSTEM_ASSERT(tokens[index] == ":", "Shader pragma is invalid");
					++index;

					const std::string_view stage = tokens[index];
					SHADER_SYSTEM_ASSERT(stage == "vertex" || stage == "fragment" || stage == "geometry" || stage == "compute" || stage == "tess_control" || stage == "tess_eval", "Invalid shader type");
					ShaderDomain shaderStage = utils::ShaderDomainFromString(std::string(stage.begin(), stage.end()));

					stagePositions.emplace_back(shaderStage, startOfStage);
				}
			}
			else if (tokens[index] == "ifdef")
			{
				++index;
				if (tokens[index].rfind(SHADER_SYSTEM_SPECIAL_MACRO_NAME, 0) == 0)
				{
					specialMacros.emplace(tokens[index]);
				}
			}
			else if (tokens[index] == "if" || tokens[index] == "define")
			{
				++index;
				for (uint64_t i = index; i < tokens.size(); ++i)
				{
					if (tokens[i].rfind(SHADER_SYSTEM_SPECIAL_MACRO_NAME, 0) == 0)
					{
						specialMacros.emplace(tokens[i]);
					}
				}
			}
			else if (Lang == ShaderLanguage::GLSL)
			{
				if (tokens[index] == "version")
				{
					++index;
					startOfStage = pos;
				}
			}

			pos = newSource.find('#', pos + 1);
		}

		SHADER_SYSTEM_ASSERT(stagePositions.size(), "Could not pre-process shader! There are no known shaders defined in file.");

		// Get first stage
		auto& [firstStage, firstStagePos] = stagePositions[0];
		if (stagePositions.size() > 1)
		{
			const std::string firstStageStr = newSource.substr(0, stagePositions[1].second);
			shaderSources[firstStage] = firstStageStr;
		}
		else
		{
			// we only have one shader source
			shaderSources[firstStage] = newSource;
		}

		for (uint64_t i = 1; i < stagePositions.size(); ++i)
		{
			auto& [stage, stagePos] = stagePositions[i];

			if ((i + 1) >= stagePositions.size())
			{
				std::string lastStageStr = newSource.substr(stagePos);
				shaderSources[stage] = lastStageStr;
				break;
			}

			std::string stageStr = newSource.substr(stagePos, stagePositions[i + 1].second - stagePos);
			shaderSources[stage] = stageStr;
		}

		// TODO: Investigate why weird characters are before the #version
		// Cleanup: make sure nothing is before a #version command
		for (auto& [stage, source] : shaderSources)
		{
			size_t versionPos = source.find("#version");
			if (versionPos != std::string::npos)
			{
				std::string newS = source.substr(versionPos);
				shaderSources[stage] = newS;
			}
		}

		return shaderSources;
	}

}
