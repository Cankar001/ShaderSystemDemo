#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRenderer.h"
#include "Platform/DX11/DX11Renderer.h"
#include "Platform/DX12/DX12Renderer.h"

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
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}

	RendererAPI::RendererAPI(RenderingAPIType inType)
		: mType(inType)
	{
	}
}
