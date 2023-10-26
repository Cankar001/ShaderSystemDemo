#include "RenderingContext.h"

// TODO: add better abstraction for each rendering api, but for now it is enough to only support OpenGL

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DX11/DX11Context.h"
#include "Platform/DX12/DX12Context.h"
#include "Platform/Metal/MetalContext.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
    Ref<RenderingContext> RenderingContext::Create(void* inWindowHandle)
    {
        switch (Renderer::GetCurrentRenderingAPIType())
        {
        default:
        case RenderingAPIType::OpenGL:
            return MakeRef<OpenGLContext>(inWindowHandle);

        case RenderingAPIType::DirectX11:
            return MakeRef<DX11Context>(inWindowHandle);

        case RenderingAPIType::DirectX12:
            return MakeRef<DX12Context>(inWindowHandle);

        case RenderingAPIType::Metal:
            return MakeRef<MetalContext>(inWindowHandle);

        case RenderingAPIType::Vulkan:
            return nullptr;
        }
    }
}
