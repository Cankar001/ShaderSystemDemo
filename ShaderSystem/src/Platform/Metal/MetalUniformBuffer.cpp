#include "MetalUniformBuffer.h"

namespace ShaderSystem
{
	MetalUniformBuffer::MetalUniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: UniformBuffer(inBinding, inLayout)
	{
	}

	MetalUniformBuffer::~MetalUniformBuffer()
	{
	}
	
	void MetalUniformBuffer::Bind() const
	{
	}
	
	void MetalUniformBuffer::Unbind() const
	{
	}
	
	void MetalUniformBuffer::UploadToShader()
	{
	}
}
