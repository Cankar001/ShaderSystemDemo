#include "Window.h"

#include "Platform/Windows/WindowImpl.h"

namespace ShaderSystem
{
	Window* Window::s_WindowInstance = nullptr;

	Window& Window::Get()
	{
		return *s_WindowInstance;
	}

	Ref<Window> Window::Create(const WindowData& properties)
	{
#if SHADER_SYSTEM_USE_NATIVE_WIN_API
		Ref<Window> window = MakeUnique<WindowsWindow>(properties);
#else
		Ref<Window> window = MakeUnique<GLFWWindow>(properties);
#endif

		if (!s_WindowInstance)
		{
			s_WindowInstance = window.get();
		}

		return window;
	}
}
