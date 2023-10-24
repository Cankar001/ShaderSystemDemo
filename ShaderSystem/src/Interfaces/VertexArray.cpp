#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/DX11/DX11VertexArray.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLVertexArray>();

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11VertexArray>();

		case RenderingAPIType::DirectX12:
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}
}
