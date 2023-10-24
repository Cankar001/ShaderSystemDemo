#include "RenderingContext.h"

// TODO: add better abstraction for each rendering api, but for now it is enough to only support OpenGL

#include "Platform/OpenGL/OpenGLContext.h"

namespace ShaderSystem
{
    Ref<RenderingContext> RenderingContext::Create(void* inWindowHandle)
    {
        return MakeRef<OpenGLContext>(inWindowHandle);
    }
}
