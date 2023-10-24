#include "DX11Context.h"

#include "Window/Window.h"

namespace ShaderSystem
{
#if SHADER_SYSTEM_USE_NATIVE_WIN_API

	DX11Context::DX11Context(void *inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}

	DX11Context::~DX11Context()
	{
	}
	
	void DX11Context::Init()
	{
	}
	
	void DX11Context::MakeCurrent()
	{
	}
	
	void *DX11Context::GetCurrentContext()
	{
		return nullptr;
	}

#else

	DX11Context::DX11Context(void *inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}

	DX11Context::~DX11Context()
	{
	}

	void DX11Context::Init()
	{
	}

	void DX11Context::MakeCurrent()
	{
	}

	void *DX11Context::GetCurrentContext()
	{
		return nullptr;
	}

#endif // SHADER_SYSTEM_USE_NATIVE_WIN_API
}
