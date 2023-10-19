#include "Window.h"

#include "Platform/Windows/WindowImpl.h"

namespace ShaderSystem
{
	Window* Window::s_WindowInstance = nullptr;

	Window& Window::Get()
	{
		return *s_WindowInstance;
	}

	UniqueRef<Window> Window::Create(const WindowData& properties)
	{
#if SHADER_SYSTEM_USE_NATIVE_WIN_API
		return MakeUnique<WindowsWindow>(properties);
#else
		return MakeUnique<GLFWWindow>(properties);
#endif
	}
}
