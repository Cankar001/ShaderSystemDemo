#include "GPUShader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace ShaderSystem
{
	Ref<GPUShader> GPUShader::Create()
	{
		return MakeRef<OpenGLShader>();
	}
}
