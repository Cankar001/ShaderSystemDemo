#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace ShaderSystem
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<int32_t> &inIndices)
	{
		mCount = (uint32_t)inIndices.size();
		mSize = ((uint32_t)inIndices.size()) * sizeof(int32_t);
		
		mData = malloc(mSize);
		memcpy(mData, &inIndices[0], mSize);

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glNamedBufferData(mRendererID, mSize, mData, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const void *inData, uint32_t inSize)
	{
		mCount = inSize / sizeof(int32_t);
		mSize = inSize;

		mData = malloc(mSize);
		memcpy(mData, inData, inSize);

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glNamedBufferData(mRendererID, mSize, mData, GL_STATIC_DRAW);
	}
	
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t inSize)
	{
		mCount = inSize / sizeof(int32_t);
		mSize = inSize;

		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glNamedBufferData(mRendererID, mSize, nullptr, GL_DYNAMIC_DRAW);
	}
	
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &mRendererID);

		if (mData)
			free(mData);
	}
	
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}
	
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void OpenGLIndexBuffer::UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
		mCount = inSize / sizeof(int32_t);
		mSize = inSize;

		if (mData)
			free(mData);

		mData = malloc(mSize);
		memcpy(mData, inData, inSize);

		glNamedBufferSubData(mRendererID, inOffset, mSize, mData);
	}
	
	void OpenGLIndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
		mCount = (uint32_t)inIndices.size();
		mSize = ((uint32_t)inIndices.size()) * sizeof(int32_t);

		if (mData)
			free(mData);

		mData = malloc(mSize);
		memcpy(mData, &inIndices[0], mSize);

		glNamedBufferSubData(mRendererID, inOffset, mSize, mData);
	}
}
