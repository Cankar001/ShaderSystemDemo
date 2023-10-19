#pragma once

#include "Core/Core.h"
#include "Shader/ShaderBuffer.h"

#include <string>
#include <unordered_map>

namespace ShaderSystem
{
	class GPUShader
	{
	public:

		virtual ~GPUShader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void ReleaseAllDomains() = 0;
		virtual bool LinkAllShaders() = 0;

		virtual void AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain) = 0;
		virtual const std::string& GetShaderDomainSource(ShaderDomain inDomain) const = 0;

		virtual void AddShaderDomain(const std::vector<uint32_t>& inSourceBinary, ShaderDomain inDomain) = 0;
		virtual const std::vector<uint32_t>& GetShaderDomainBinary(ShaderDomain inDomain) const = 0;

		static Ref<GPUShader> Create();

	protected:

		std::unordered_map<ShaderDomain, std::string> mTextDomains;
		std::unordered_map<ShaderDomain, std::vector<uint32_t>> mBinaryDomains;
	};
}
