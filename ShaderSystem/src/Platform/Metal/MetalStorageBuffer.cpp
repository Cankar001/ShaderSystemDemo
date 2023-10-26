#include "MetalStorageBuffer.h"

namespace ShaderSystem
{
	MetalStorageBuffer::MetalStorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: StorageBuffer(inBinding, inLayout)
	{
	}
	
	MetalStorageBuffer::~MetalStorageBuffer()
	{
	}
	
	void MetalStorageBuffer::Bind() const
	{
	}
	
	void MetalStorageBuffer::Unbind() const
	{
	}
	
	void MetalStorageBuffer::UploadToShader()
	{
	}
	
	void MetalStorageBuffer::Resize(uint32_t inSize)
	{
	}
}
