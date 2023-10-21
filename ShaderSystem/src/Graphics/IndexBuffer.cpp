#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace ShaderSystem
{
	Ref<IndexBuffer> IndexBuffer::Create(const std::vector<int32_t> &inIndices)
	{
		return MakeRef<OpenGLIndexBuffer>(inIndices);
	}

	Ref<IndexBuffer> IndexBuffer::Create(const void *inData, uint32_t inSize)
	{
		return MakeRef<OpenGLIndexBuffer>(inData, inSize);
	}
	
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t inSize)
	{
		return MakeRef<OpenGLIndexBuffer>(inSize);
	}
}
