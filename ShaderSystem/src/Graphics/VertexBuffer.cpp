#include "VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace ShaderSystem
{
	Ref<VertexBuffer> VertexBuffer::Create(void *inData, uint32_t inSize)
	{
		return MakeRef<OpenGLVertexBuffer>(inData, inSize);
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t inSize)
	{
		return MakeRef<OpenGLVertexBuffer>(inSize);
	}
}
