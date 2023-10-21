#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRenderer.h"

namespace ShaderSystem
{
	Ref<RendererAPI> RendererAPI::Create()
	{
		return MakeRef<OpenGLRenderer>();
	}
}
