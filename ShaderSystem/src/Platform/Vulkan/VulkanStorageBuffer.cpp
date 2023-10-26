#include "VulkanStorageBuffer.h"

namespace ShaderSystem
{
	VulkanStorageBuffer::VulkanStorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: StorageBuffer(inBinding, inLayout)
	{
	}

	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
	}
	
	void VulkanStorageBuffer::Bind() const
	{
	}
	
	void VulkanStorageBuffer::Unbind() const
	{
	}
	
	void VulkanStorageBuffer::UploadToShader()
	{
	}
	
	void VulkanStorageBuffer::Resize(uint32_t inSize)
	{
	}
}
