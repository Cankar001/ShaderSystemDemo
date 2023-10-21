#pragma once

#include "Core/Core.h"

#include <string>
#include <filesystem>

namespace ShaderSystem
{
	class ShaderCache
	{
	public:

		static void Init();
		static void Shutdown();

		static bool HasChanged(const std::filesystem::path& inFilePath, const std::string& inSource);
	};
}
