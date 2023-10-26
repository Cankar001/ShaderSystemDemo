#include "MetalShader.h"

namespace ShaderSystem
{
	MetalShader::MetalShader(const BufferLayout &inLayout)
	{
	}

	MetalShader::~MetalShader()
	{
	}
	
	void MetalShader::Bind() const
	{
	}
	
	void MetalShader::Unbind()
	{
	}
	
	uint32_t MetalShader::GetRendererID() const
	{
		return 0;
	}
	
	void MetalShader::ReleaseAllDomains()
	{
	}
	
	bool MetalShader::LinkAllShaders()
	{
		return true;
	}
	
	void MetalShader::AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain)
	{
	}
	
	const std::string &MetalShader::GetShaderDomainSource(ShaderDomain inDomain) const
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
	
	void MetalShader::AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain)
	{
	}
	
	const std::vector<uint32_t> &MetalShader::GetShaderDomainBinary(ShaderDomain inDomain) const
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
