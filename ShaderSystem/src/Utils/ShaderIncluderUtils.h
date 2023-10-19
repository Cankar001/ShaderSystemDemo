#pragma once

#include "Core/Core.h"

#include <fstream>

namespace ShaderSystem
{
	namespace utils
	{
		static int32_t SkipBOM(std::istream& inStream)
		{
			char test[4] = { 0 };
			inStream.seekg(0, std::ios::beg);
			inStream.read(test, 3);

			if (strcmp(test, "\xEF\xBB\xBF") == 0)
			{
				inStream.seekg(3, std::ios::beg);
				return 3;
			}

			inStream.seekg(0, std::ios::beg);
			return 0;
		}

		static std::string ReadFileAndSkipBOM(const std::filesystem::path& inFilePath)
		{
			std::string result;
			std::ifstream inStream(inFilePath, std::ios::in | std::ios::binary);
			if (inStream)
			{
				inStream.seekg(0, std::ios::end);
				uint32_t fileSize = (uint32_t)inStream.tellg();
				const int32_t skippedChars = SkipBOM(inStream);

				fileSize -= skippedChars - 1;
				result.resize(fileSize);
				inStream.read(result.data() + 1, fileSize);

				// NOTE: Add a dummy tab to beginning of file.
				result[0] = '\t';
			}

			inStream.close();
			return result;
		}
	}
}
