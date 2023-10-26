#include "VulkanUniformBuffer.h"

namespace ShaderSystem
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: UniformBuffer(inBinding, inLayout)
	{
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
	}
	
	void VulkanUniformBuffer::Bind() const
	{
	}
	
	void VulkanUniformBuffer::Unbind() const
	{
	}
	
	void VulkanUniformBuffer::UploadToShader()
	{
	}
}
