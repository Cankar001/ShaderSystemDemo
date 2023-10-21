#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ShaderSystem
{
	Ref<VertexArray> VertexArray::Create()
	{
		return MakeRef<OpenGLVertexArray>();
	}
}
