#include "DX11UniformBuffer.h"

namespace ShaderSystem
{
	DX11UniformBuffer::DX11UniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: UniformBuffer(inBinding, inLayout)
	{
	}

	DX11UniformBuffer::~DX11UniformBuffer()
	{
	}
	
	void DX11UniformBuffer::Bind() const
	{
	}
	
	void DX11UniformBuffer::Unbind() const
	{
	}
	
	void DX11UniformBuffer::UploadToShader()
	{
	}
}
