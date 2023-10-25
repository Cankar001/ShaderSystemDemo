#include "DX12StorageBuffer.h"

namespace ShaderSystem
{
	DX12StorageBuffer::DX12StorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: StorageBuffer(inBinding, inLayout)
	{
	}

	DX12StorageBuffer::~DX12StorageBuffer()
	{
	}

	void DX12StorageBuffer::Bind() const
	{
	}

	void DX12StorageBuffer::Unbind() const
	{
	}

	void DX12StorageBuffer::UploadToShader()
	{
	}

	void DX12StorageBuffer::Resize(uint32_t inSize)
	{
	}
}
