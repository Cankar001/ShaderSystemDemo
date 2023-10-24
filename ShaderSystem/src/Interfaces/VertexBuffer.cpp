#include "VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/DX11/DX11VertexBuffer.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	Ref<VertexBuffer> VertexBuffer::Create(void *inData, uint32_t inSize)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLVertexBuffer>(inData, inSize);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11VertexBuffer>(inData, inSize);

		case RenderingAPIType::DirectX12:
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t inSize)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLVertexBuffer>(inSize);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11VertexBuffer>(inSize);

		case RenderingAPIType::DirectX12:
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}
}
