#include "GLSLIncluder.h"

#include "Core/Logger.h"
#include "Core/Hash.h"
#include "Utils/ShaderIncluderUtils.h"

namespace ShaderSystem
{
	GLSLIncluder::GLSLIncluder(const shaderc_util::FileFinder* inFileFinder)
		: mFileFinder(*inFileFinder)
	{
	}

	GLSLIncluder::~GLSLIncluder()
	{
	}
	
	shaderc_include_result* GLSLIncluder::GetInclude(const char* requestedPath, shaderc_include_type type, const char* requestingPath, size_t includeDepth)
	{
		shaderc_include_result* result = new shaderc_include_result();
		std::filesystem::path requestedFullPath = (type == shaderc_include_type_relative) ? mFileFinder.FindRelativeReadableFilepath(requestingPath, requestedPath) : mFileFinder.FindReadableFilepath(requestedPath);
		auto& [source, sourceHash, shaderType, isGuarded] = mHeaderCache[requestedFullPath.string()];

		if (source.empty())
		{
			source = utils::ReadFileAndSkipBOM(requestedFullPath);
			if (source.empty())
				SHADER_SYSTEM_ERROR("Failed to load included file: {0} in {1}", requestedFullPath.string().c_str(), requestingPath);

			sourceHash = HashString(source);
			shaderType = ShaderPreProcessor::PreprocessHeader<ShaderLanguage::GLSL>(source, isGuarded, mParsedSpecialMacros, mIncludeData, requestedFullPath);
		}
		else if (isGuarded)
		{
			source.clear();
		}

		IncludeData inc;
		inc.IncludedFilePath = requestedFullPath;
		inc.IncludeDepth = includeDepth;
		inc.IsRelative = type == shaderc_include_type_relative;
		inc.IsGuarded = isGuarded;
		inc.HashValue = sourceHash;
		inc.IncludedType = shaderType;
		mIncludeData.emplace(inc);

		std::array<std::string, 2>* const container = new std::array<std::string, 2>();
		(*container)[0] = requestedPath;
		(*container)[1] = source.empty() ? "" : source.c_str();

		result->user_data = container;

		result->source_name = (*container)[0].data();
		result->source_name_length = (*container)[0].size();

		result->content = (*container)[1].data();
		result->content_length = (*container)[1].size();

		return result;
	}
	
	void GLSLIncluder::ReleaseInclude(shaderc_include_result* result)
	{
		delete static_cast<std::array<std::string, 2>*>(result->user_data);
		delete result;
	}
}
