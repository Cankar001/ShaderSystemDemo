#include "VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/DX11/DX11VertexBuffer.h"
#include "Platform/DX12/DX12VertexBuffer.h"
#include "Platform/Metal/MetalVertexBuffer.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"
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
			return MakeRef<DX12VertexBuffer>(inData, inSize);

		case RenderingAPIType::Metal:
			return MakeRef<MetalVertexBuffer>(inData, inSize);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanVertexBuffer>(inData, inSize);
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
			return MakeRef<DX12VertexBuffer>(inSize);

		case RenderingAPIType::Metal:
			return MakeRef<MetalVertexBuffer>(inSize);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanVertexBuffer>(inSize);
		}
	}
}
