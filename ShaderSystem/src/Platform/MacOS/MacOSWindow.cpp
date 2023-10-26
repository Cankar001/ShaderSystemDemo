#include "MacOSWindow.h"

#ifdef SHADER_SYSTEM_PLATFORM_MACOS

#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Events/WindowEvent.h"

namespace ShaderSystem
{
	MacOSWindow::MacOSWindow(const WindowData &properties)
		: mWindowData(properties)
	{
	}

	MacOSWindow::~MacOSWindow()
	{
	}
	
	void MacOSWindow::SetEventCallback(const EventCallbackFn &callback)
	{
		mWindowData.EventCallback = callback;
	}
	
	void MacOSWindow::Update()
	{
	}
	
	void MacOSWindow::SetWindowIcon(const std::string &path, bool flip)
	{
	}
	
	std::pair<int32_t, int32_t> MacOSWindow::GetWindowDimensions()
	{
		return std::pair<int32_t, int32_t>();
	}
	
	std::pair<int32_t, int32_t> MacOSWindow::GetWindowPosition()
	{
		return std::pair<int32_t, int32_t>();
	}
	
	void MacOSWindow::CloseWindow()
	{
		// TODO: actually close the window using the native window api.

		// Notify the app that the window is about to close.
		WindowCloseEvent e;
		if (mWindowData.EventCallback)
			mWindowData.EventCallback(e);
	}
	
	void MacOSWindow::SetVSync(bool bEnabled)
	{
		mWindowData.VSync = bEnabled;

	}
	
	void MacOSWindow::SetVisible(bool bVisible)
	{
		mWindowData.Visible = bVisible;

	}
	
	void MacOSWindow::SetFocus(bool bEnabled)
	{
		mWindowData.Focused = bEnabled;

	}
	
	void MacOSWindow::SetFullscreen(bool bEnabled)
	{
		mWindowData.Fullscreen = bEnabled;

	}
	
	void MacOSWindow::SetResizable(bool bEnabled)
	{
		// TODO: implement
	}
	
	void MacOSWindow::ShowCursor()
	{
		mWindowData.CursorVisible = true;

	}
	
	void MacOSWindow::HideCursor()
	{
		mWindowData.CursorVisible = false;

	}
	
	void MacOSWindow::Maximize()
	{
		mWindowData.Maximized = true;

	}
	
	void MacOSWindow::CenterWindow()
	{
		mWindowData.Centered = true;

	}
	
	void MacOSWindow::SetTitle(const std::string &title)
	{
		mWindowData.Title = title;

	}
	
	void *MacOSWindow::GetNativeHandle()
	{
		return nullptr;
	}
	
	void *MacOSWindow::GetNativeContext()
	{
		return nullptr;
	}
	
	void *MacOSWindow::GetNativeCursor()
	{
		return nullptr;
	}
}

#endif // SHADER_SYSTEM_PLATFORM_MACOS
