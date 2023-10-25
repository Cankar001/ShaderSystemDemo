#include "DX12IndexBuffer.h"

namespace ShaderSystem
{
	DX12IndexBuffer::DX12IndexBuffer(const std::vector<int32_t> &inIndices)
	{
	}

	DX12IndexBuffer::DX12IndexBuffer(const void *inData, uint32_t inSize)
	{
	}
	
	DX12IndexBuffer::DX12IndexBuffer(uint32_t inSize)
	{
	}
	
	DX12IndexBuffer::~DX12IndexBuffer()
	{
	}
	
	void DX12IndexBuffer::Bind() const
	{
	}
	
	void DX12IndexBuffer::Unbind() const
	{
	}
	
	void DX12IndexBuffer::UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
	}
	
	void DX12IndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
	}
}
