#pragma once

#include "Core/Core.h"
#include "Shader.h"

#include <unordered_map>
#include <string>
#include <filesystem>

namespace ShaderSystem
{
	class ShaderLibrary
	{
	public:

		ShaderLibrary();
		~ShaderLibrary();

		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Load(const std::filesystem::path& filePath, bool forceCompile = false);
		void Load(const std::string& name, const std::filesystem::path& filePath, bool forceCompile = false);

		void LoadFromString(const std::string& source, ShaderLanguage language = ShaderLanguage::GLSL);
		void LoadFromString(const std::string& name, const std::string& source, ShaderLanguage language = ShaderLanguage::GLSL);

		void ReloadShader(const std::string& name);
		void ReloadAllShaders();

		void Remove(const std::string& name);

		const Ref<Shader>& Get(const std::string& name);

	private:

		std::unordered_map<std::string, Ref<Shader>> mShaders;
	};
}
