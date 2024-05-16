#include "VulkanShader.h"

namespace ShaderSystem
{
	VulkanShader::VulkanShader(const BufferLayout &inLayout)
	{
	}

	VulkanShader::~VulkanShader()
	{
	}
	
	void VulkanShader::Bind(ShaderDomain inDomain) const
	{
	}
	
	void VulkanShader::Unbind()
	{
	}
	
	uint32_t VulkanShader::GetRendererID() const
	{
		return 0;
	}
	
	void VulkanShader::ReleaseAllDomains()
	{
	}
	
	bool VulkanShader::LinkAllShaders()
	{
		return true;
	}
	
	void VulkanShader::AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain)
	{
	}
	
	const std::string &VulkanShader::GetShaderDomainSource(ShaderDomain inDomain) const
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
	
	void VulkanShader::AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain)
	{
	}
	
	const std::vector<uint32_t> &VulkanShader::GetShaderDomainBinary(ShaderDomain inDomain) const
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
