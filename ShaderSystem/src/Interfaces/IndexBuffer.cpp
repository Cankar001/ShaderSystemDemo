#include "IndexBuffer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Platform/DX11/DX11IndexBuffer.h"
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
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
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
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
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
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}
}
