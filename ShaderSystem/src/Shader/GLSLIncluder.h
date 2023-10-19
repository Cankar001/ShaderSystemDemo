#pragma once

#include "Core/Core.h"
#include "ShaderPreProcessor.h"

#include <shaderc/shaderc.hpp>
#include <libshaderc_util/file_finder.h>

#include <string>

namespace ShaderSystem
{
	class GLSLIncluder : public shaderc::CompileOptions::IncluderInterface
	{
	public:

		explicit GLSLIncluder(const shaderc_util::FileFinder* inFileFinder);
		virtual ~GLSLIncluder();

		shaderc_include_result* GetInclude(const char* requestedPath, shaderc_include_type type, const char* requestingPath, size_t includeDepth) override;
		void ReleaseInclude(shaderc_include_result* result) override;

		std::unordered_set<IncludeData>&& GetIncludeData() { return std::move(mIncludeData); }
		std::unordered_set<std::string>&& GetParsedSpecialMacros() { return std::move(mParsedSpecialMacros); }

	private:

		const shaderc_util::FileFinder& mFileFinder;
		std::unordered_set<IncludeData> mIncludeData;
		std::unordered_set<std::string> mParsedSpecialMacros;
		std::unordered_map<std::string, HeaderCache> mHeaderCache;
	};
}
