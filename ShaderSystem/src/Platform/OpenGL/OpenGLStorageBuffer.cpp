#include "OpenGLStorageBuffer.h"

#include <glad/glad.h>

namespace ShaderSystem
{
	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: StorageBuffer(inBinding, inLayout)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, inSize, mGPUData, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, inBinding, mRendererID);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}
	
	void OpenGLStorageBuffer::Bind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, mBinding, mRendererID, 0, mGPUDataSize);
	}
	
	void OpenGLStorageBuffer::Unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	
	void OpenGLStorageBuffer::UploadToShader()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mGPUDataSize, mGPUData);
	}
	
	void OpenGLStorageBuffer::Resize(uint32_t inSize)
	{
		// TODO: actually resize the buffer

		if (inSize != mGPUDataSize)
		{
			mGPUDataSize = inSize;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mGPUDataSize, mGPUData);
		}
	}
}
