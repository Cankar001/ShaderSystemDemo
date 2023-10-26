#include "MetalIndexBuffer.h"

namespace ShaderSystem
{
	MetalIndexBuffer::MetalIndexBuffer(const std::vector<int32_t> &inIndices)
	{
	}

	MetalIndexBuffer::MetalIndexBuffer(const void *inData, uint32_t inSize)
	{
	}
	
	MetalIndexBuffer::MetalIndexBuffer(uint32_t inSize)
	{
	}
	
	MetalIndexBuffer::~MetalIndexBuffer()
	{
	}
	
	void MetalIndexBuffer::Bind() const
	{
	}
	
	void MetalIndexBuffer::Unbind() const
	{
	}
	
	void MetalIndexBuffer::UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
	}
	
	void MetalIndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
	}
}
