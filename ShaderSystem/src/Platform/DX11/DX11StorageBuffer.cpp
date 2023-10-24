#include "DX11StorageBuffer.h"

namespace ShaderSystem
{
	DX11StorageBuffer::DX11StorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: StorageBuffer(inBinding, inLayout)
	{
	}

	DX11StorageBuffer::~DX11StorageBuffer()
	{
	}
	
	void DX11StorageBuffer::Bind() const
	{
	}
	
	void DX11StorageBuffer::Unbind() const
	{
	}
	
	void DX11StorageBuffer::UploadToShader()
	{
	}
	
	void DX11StorageBuffer::Resize(uint32_t inSize)
	{
	}
}
