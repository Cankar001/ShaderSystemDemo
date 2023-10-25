#include "DX12UniformBuffer.h"

namespace ShaderSystem
{
	DX12UniformBuffer::DX12UniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: UniformBuffer(inBinding, inLayout)
	{
	}
	
	DX12UniformBuffer::~DX12UniformBuffer()
	{
	}
	
	void DX12UniformBuffer::Bind() const
	{
	}
	
	void DX12UniformBuffer::Unbind() const
	{
	}
	
	void DX12UniformBuffer::UploadToShader()
	{
	}
}
