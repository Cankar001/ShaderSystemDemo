#include "DX12Shader.h"

namespace ShaderSystem
{
	void DX12Shader::Bind() const
	{
	}

	void DX12Shader::Unbind()
	{
	}
	
	uint32_t DX12Shader::GetRendererID() const
	{
		return 0;
	}
	
	void DX12Shader::ReleaseAllDomains()
	{
	}
	
	bool DX12Shader::LinkAllShaders()
	{
		return true;
	}

	void DX12Shader::AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain)
	{
	}
	
	const std::string &DX12Shader::GetShaderDomainSource(ShaderDomain inDomain) const
	{
		for (const auto &[d, s] : mTextDomains)
		{
			if (d == inDomain)
			{
				return s;
			}
		}

		return std::string();
	}
	
	void DX12Shader::AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain)
	{
	}
	
	const std::vector<uint32_t> &DX12Shader::GetShaderDomainBinary(ShaderDomain inDomain) const
	{
		for (const auto &[d, b] : mBinaryDomains)
		{
			if (d == inDomain)
			{
				return b;
			}
		}

		return std::vector<uint32_t>();
	}
}
