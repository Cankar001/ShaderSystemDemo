#pragma once

#include "Interfaces/GPUShader.h"

#include "Core/Core.h"

#include "ShaderBuffer.h"
#include "ShaderPreProcessor.h"

#include <map>
#include <unordered_set>
#include <functional>
#include <string>
#include <filesystem>

#include <spirv_cross/spirv_glsl.hpp>

namespace ShaderSystem
{
	class Shader
	{
	public:

		using ShaderReloadedCallback = std::function<void()>;

		Shader(const std::filesystem::path& inFilePath, bool inForceCompile);
		Shader(const std::string& inSource, const std::string& inName, ShaderLanguage inLanguage);
		~Shader();

		void Reload(bool inForceCompile = false);
		void Release();
		uint64_t GetHash() const;

		void Bind() const;
		void Unbind();
		uint32_t GetRendererID();

		void AddShaderReloadedCallback(const ShaderReloadedCallback& inCallback);
		void SetMacro(const std::string& inName, const std::string& inValue);

		const std::string& GetName() const { return mName; }
		const std::filesystem::path& GetFilePath() const { return mFilePath; }

		static Ref<Shader> LoadFromFile(const std::filesystem::path& inFilePath, bool inForceCompile = false);
		static Ref<Shader> LoadFromString(const std::string &inSource, const std::string &inName = "undefined", ShaderLanguage inLanguage = ShaderLanguage::GLSL);

	private:

		void Load(const std::string& inSource, bool inForceCompile);

		// Pre-Processing
		std::unordered_map<ShaderDomain, std::string> PreProcess(const std::string& inSource);
		std::unordered_map<ShaderDomain, std::string> PreProcessGLSL(const std::string& inSource);
		std::unordered_map<ShaderDomain, std::string> PreProcessHLSL(const std::string& inSource);

		// Shader-Reflection
		void ReflectAllShaderStages(const std::unordered_map<ShaderDomain, std::vector<uint32_t>>& inShaderData);
		void Reflect(ShaderDomain shaderStage, const std::vector<uint32_t>& inShaderData);
		void LoadCachedReflectionData();

		// Shader-compilation
		std::string Compile(std::unordered_map<ShaderDomain, std::vector<uint32_t>>& outOutputBinary, const ShaderDomain inDomain) const;
		void CompileOrGetOpenGLBinary(std::unordered_map<ShaderDomain, std::vector<uint32_t>>& outBinary, bool inForceCompile);
		void TryGetOpenGLCachedBinary(const std::filesystem::path& inCacheDirectory, const std::string& inExtension, std::unordered_map<ShaderDomain, std::vector<uint32_t>>& outBinary, ShaderDomain inDomain) const;
		void LoadAndCreateShaders(const std::unordered_map<ShaderDomain, std::vector<uint32_t>>& inShaderData);

		void ParseConstantBuffers(const spirv_cross::CompilerGLSL& compiler);

	protected:

		std::string mName;
		std::filesystem::path mFilePath;

		bool mLoaded = false;
		bool mIsCompute = false;
		ShaderLanguage mLanguage = ShaderLanguage::GLSL;
		uint32_t mConstantBufferOffset = 0;

		std::unordered_map<std::string, std::string> mMacros;
		std::unordered_map<std::string, int32_t> mUniformLocations;
		std::unordered_set<std::string> mAcknowledgedMacros;

		std::unordered_map<ShaderDomain, std::string> mShaderSources;
		std::unordered_map<std::string, ShaderBuffer> mBuffers;
		std::unordered_map<std::string, ShaderResourceDeclaration> mResources;
		std::vector<ShaderReloadedCallback> mReloadedCallbacks;
		std::map<ShaderDomain, StageData> mStagesMetaData;

		std::vector<ShaderDescriptorSet> mShaderDescriptorSets;
		std::vector<ShaderPushConstantRange> mPushConstantRanges;

		//!< the shader instance which contains the real rendering api specific shader handling.
		Ref<GPUShader> mGpuShader = nullptr;
	};
}
