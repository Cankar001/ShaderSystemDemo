#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRenderer.h"
#include "Platform/DX11/DX11Renderer.h"
#include "Platform/DX12/DX12Renderer.h"
#include "Platform/Metal/MetalRenderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"

namespace ShaderSystem
{
	Ref<RendererAPI> RendererAPI::Create(RenderingAPIType inType)
	{
		switch (inType)
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLRenderer>(inType);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11Renderer>(inType);

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12Renderer>(inType);

		case RenderingAPIType::Metal:
			return MakeRef<MetalRenderer>(inType);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanRenderer>(inType);
		}
	}

	RendererAPI::RendererAPI(RenderingAPIType inType)
		: mType(inType)
	{
	}
}
