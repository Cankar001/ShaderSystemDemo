#pragma once

#include "Core/Core.h"
#include "Interfaces/GPUShader.h"

#include "Graphics/BufferLayout.h"

namespace ShaderSystem
{
	class DX12Shader : public GPUShader
	{
	public:

		DX12Shader(const BufferLayout &inLayout);
		virtual ~DX12Shader();

		virtual void Bind(ShaderDomain inDomain) const override;
		virtual void Unbind() override;
		virtual uint32_t GetRendererID() const override;

		virtual void ReleaseAllDomains() override;
		virtual bool LinkAllShaders() override;

		virtual void AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain) override;
		virtual const std::string &GetShaderDomainSource(ShaderDomain inDomain) const override;

		virtual void AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain) override;
		virtual const std::vector<uint32_t> &GetShaderDomainBinary(ShaderDomain inDomain) const override;

	private:

	};
}
