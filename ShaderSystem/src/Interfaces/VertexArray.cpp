#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/DX11/DX11VertexArray.h"
#include "Platform/DX12/DX12VertexArray.h"
#include "Platform/Metal/MetalVertexArray.h"
#include "Platform/Vulkan/VulkanVertexArray.h"
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
			return MakeRef<DX12VertexArray>();

		case RenderingAPIType::Metal:
			return MakeRef<MetalVertexArray>();

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanVertexArray>();
		}
	}
}
