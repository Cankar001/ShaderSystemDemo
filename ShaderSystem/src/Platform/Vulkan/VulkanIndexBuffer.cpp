#include "VulkanIndexBuffer.h"

namespace ShaderSystem
{
	VulkanIndexBuffer::VulkanIndexBuffer(const std::vector<int32_t> &inIndices)
	{
	}

	VulkanIndexBuffer::VulkanIndexBuffer(const void *inData, uint32_t inSize)
	{
	}
	
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t inSize)
	{
	}
	
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}
	
	void VulkanIndexBuffer::Bind() const
	{
	}
	
	void VulkanIndexBuffer::Unbind() const
	{
	}
	
	void VulkanIndexBuffer::UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
	}
	
	void VulkanIndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
	}
}
