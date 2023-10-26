#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/DX11/DX11IndexBuffer.h"
#include "Platform/DX12/DX12IndexBuffer.h"
#include "Platform/Metal/MetalIndexBuffer.h"
#include "Platform/Vulkan/VulkanIndexBuffer.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	Ref<IndexBuffer> IndexBuffer::Create(const std::vector<int32_t> &inIndices)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLIndexBuffer>(inIndices);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11IndexBuffer>(inIndices);

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12IndexBuffer>(inIndices);

		case RenderingAPIType::Metal:
			return MakeRef<MetalIndexBuffer>(inIndices);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanIndexBuffer>(inIndices);
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(const void *inData, uint32_t inSize)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLIndexBuffer>(inData, inSize);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11IndexBuffer>(inData, inSize);

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12IndexBuffer>(inData, inSize);

		case RenderingAPIType::Metal:
			return MakeRef<MetalIndexBuffer>(inData, inSize);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanIndexBuffer>(inData, inSize);
		}
	}
	
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t inSize)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLIndexBuffer>(inSize);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11IndexBuffer>(inSize);

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12IndexBuffer>(inSize);

		case RenderingAPIType::Metal:
			return MakeRef<MetalIndexBuffer>(inSize);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanIndexBuffer>(inSize);
		}
	}
}
