#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace ShaderSystem
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: UniformBuffer(inBinding, inLayout)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glBufferData(GL_UNIFORM_BUFFER, mGPUDataSize, mGPUData, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, mBinding, mRendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}
	
	void OpenGLUniformBuffer::Bind() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, mBinding, mRendererID);
	}
	
	void OpenGLUniformBuffer::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
	void OpenGLUniformBuffer::UploadToShader()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, mGPUDataSize, mGPUData);
	}
}
