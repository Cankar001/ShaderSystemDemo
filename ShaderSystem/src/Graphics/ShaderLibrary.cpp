#include "ShaderLibrary.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

namespace ShaderSystem
{
	ShaderLibrary::ShaderLibrary() {}
	ShaderLibrary::~ShaderLibrary() {}
	
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		SHADER_SYSTEM_ASSERT(mShaders.find(name) == mShaders.end(), fmt::format("Shader {0} already exists!", name.c_str()).c_str());
		mShaders[name] = shader;

		SHADER_SYSTEM_INFO("Added shader '{0}'", name.c_str());
	}
	
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		SHADER_SYSTEM_ASSERT(mShaders.find(name) == mShaders.end(), fmt::format("Shader {0} already exists!", name.c_str()).c_str());
		mShaders[name] = shader;

		SHADER_SYSTEM_INFO("Added shader '{0}'", name.c_str());
	}
	
	void ShaderLibrary::Load(const std::filesystem::path& filePath, const BufferLayout &inLayout, bool forceCompile)
	{
		Ref<Shader> shader = Shader::LoadFromFile(filePath, inLayout, forceCompile);
		if (!shader)
		{
			SHADER_SYSTEM_ERROR("Failed to load shader '{0}' into shaderlibrary!", shader->GetName().c_str());
			return;
		}
	
		Add(shader);
	}
	
	void ShaderLibrary::Load(const std::string& name, const std::filesystem::path& filePath, const BufferLayout &inLayout, bool forceCompile)
	{
		Ref<Shader> shader = Shader::LoadFromFile(filePath, inLayout, forceCompile);
		if (!shader)
		{
			SHADER_SYSTEM_ERROR("Failed to load shader '{0}' into shaderlibrary!", name.c_str());
			return;
		}
	
		Add(name, shader);
	}
	
	void ShaderLibrary::LoadFromString(const std::string& source, const BufferLayout &inLayout, ShaderLanguage language)
	{
		Ref<Shader> shader = Shader::LoadFromString(source, "undefined", inLayout, language);
		if (!shader)
		{
			SHADER_SYSTEM_ERROR("Failed to load shader '{0}' into shaderlibrary!", shader->GetName().c_str());
			return;
		}
	
		Add(shader);
	}
	
	void ShaderLibrary::LoadFromString(const std::string& name, const std::string& source, const BufferLayout &inLayout, ShaderLanguage language)
	{
		Ref<Shader> shader = Shader::LoadFromString(source, name, inLayout, language);
		if (!shader)
		{
			SHADER_SYSTEM_ERROR("Failed to load shader '{0}' into shaderlibrary!", name.c_str());
			return;
		}
	
		Add(name, shader);
	}
	
	void ShaderLibrary::ReloadShader(const std::string& name)
	{
		SHADER_SYSTEM_ASSERT(mShaders.find(name) != mShaders.end(), fmt::format("Could not find shader {0}", name.c_str()).c_str());
		mShaders[name]->Reload();
	}
	
	void ShaderLibrary::ReloadAllShaders()
	{
		for (auto& [name, shader] : mShaders)
		{
			shader->Reload();
		}
	}
	
	void ShaderLibrary::Remove(const std::string& name)
	{
		SHADER_SYSTEM_ASSERT(mShaders.find(name) != mShaders.end(), fmt::format("Could not find shader {0}", name.c_str()).c_str());
		mShaders[name]->Release();
		mShaders.erase(name);
	}
	
	const Ref<Shader>& ShaderLibrary::Get(const std::string& name)
	{
		SHADER_SYSTEM_ASSERT(mShaders.find(name) != mShaders.end(), fmt::format("Could not find shader {0}", name.c_str()).c_str());
		return mShaders[name];
	}
}
