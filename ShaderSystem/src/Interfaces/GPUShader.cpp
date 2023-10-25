#include "GPUShader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/DX11/DX11Shader.h"
#include "Platform/DX12/DX12Shader.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	Ref<GPUShader> GPUShader::Create()
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLShader>();

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11Shader>();

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12Shader>();

		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}
}
