#include "DX11Shader.h"

namespace ShaderSystem
{
	DX11Shader::DX11Shader()
	{
	}

	DX11Shader::~DX11Shader()
	{
	}
	
	void DX11Shader::Bind() const
	{
	}
	
	void DX11Shader::Unbind()
	{
	}
	
	uint32_t DX11Shader::GetRendererID() const
	{
		return 0;
	}
	
	void DX11Shader::ReleaseAllDomains()
	{
	}
	
	bool DX11Shader::LinkAllShaders()
	{
		return true;
	}

	void DX11Shader::AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain)
	{
	}

	const std::string &DX11Shader::GetShaderDomainSource(ShaderDomain inDomain) const
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

	void DX11Shader::AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain)
	{
	}

	const std::vector<uint32_t> &DX11Shader::GetShaderDomainBinary(ShaderDomain inDomain) const
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
