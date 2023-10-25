#include "GPUShader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/DX11/DX11Shader.h"
#include "Platform/DX12/DX12Shader.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	Ref<GPUShader> GPUShader::Create(const BufferLayout &inLayout)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLShader>(inLayout);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11Shader>(inLayout);

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12Shader>(inLayout);

		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}
}
