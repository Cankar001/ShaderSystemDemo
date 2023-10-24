#include "DX11IndexBuffer.h"

namespace ShaderSystem
{
	DX11IndexBuffer::DX11IndexBuffer(const std::vector<int32_t> &inIndices)
	{
	}

	DX11IndexBuffer::DX11IndexBuffer(const void *inData, uint32_t inSize)
	{
	}
	
	DX11IndexBuffer::DX11IndexBuffer(uint32_t inSize)
	{
	}
	
	DX11IndexBuffer::~DX11IndexBuffer()
	{
	}
	
	void DX11IndexBuffer::Bind() const
	{
	}
	
	void DX11IndexBuffer::Unbind() const
	{
	}
	
	void DX11IndexBuffer::UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
	}
	
	void DX11IndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
	}
}
