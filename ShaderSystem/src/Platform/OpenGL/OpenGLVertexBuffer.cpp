#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace ShaderSystem
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(void *inData, uint32_t inSize)
		: mSize(inSize)
	{
		mData = malloc(inSize);
		memcpy(mData, inData, inSize);

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glNamedBufferData(mRendererID, mSize, mData, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t inSize)
		: mData(nullptr), mSize(inSize)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glNamedBufferData(mRendererID, mSize, nullptr, GL_DYNAMIC_DRAW);
	}
	
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &mRendererID);

		if (mData)
			free(mData);
	}
	
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}
	
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void OpenGLVertexBuffer::UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		mSize = inSize;
		memcpy(mData, ((Byte*)inData + inOffset), inSize);

		glNamedBufferSubData(mRendererID, inOffset, mSize, mData);
	}
}
