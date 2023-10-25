#include "WindowsWindow.h"

#ifdef SHADER_SYSTEM_PLATFORM_WINDOWS

#include "Core/Assert.h"
#include "Core/Logger.h"

#include "Events/WindowEvent.h"

#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	WindowsWindow::WindowsWindow(const WindowData& properties)
		: mWindowData(properties)
	{
		Window::s_WindowInstance = this;
		Init();
	}

	WindowsWindow::~WindowsWindow()
	{
		if (mNativeHandle)
			DestroyWindow(mNativeHandle);
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		mWindowData.EventCallback = callback;
	}

	const EventCallbackFn& WindowsWindow::GetEventCallback() const
	{
		return mWindowData.EventCallback;
	}

	void WindowsWindow::Update()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (PeekMessage(&msg, mNativeHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_NULL)
		{
			if (!IsWindow(mNativeHandle))
			{
				mNativeHandle = NULL;
				return;
			}
		}

		SwapBuffers((HDC)mContext->GetCurrentContext());

		mContext->SwapBuffers();
	}
	
	void WindowsWindow::SetWindowIcon(const std::string& path, bool flip)
	{
	}
	
	std::pair<int32_t, int32_t> WindowsWindow::GetWindowDimensions()
	{
		HDC windowHDC = GetDC(mNativeHandle);
		int32_t width = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
		int32_t height = GetDeviceCaps(windowHDC, DESKTOPVERTRES);
		return { width, height };
	}
	
	std::pair<int32_t, int32_t> WindowsWindow::GetWindowPosition()
	{
		RECT windowRect;
		GetWindowRect(mNativeHandle, &windowRect);
		return { windowRect.left, windowRect.top };
	}
	
	void WindowsWindow::CloseWindow()
	{
		WindowCloseEvent event;
		mWindowData.EventCallback(event);
		PostQuitMessage(0);
	}
	
	void WindowsWindow::SetVSync(bool bEnabled)
	{
		mWindowData.VSync = bEnabled;
		((BOOL(WINAPI*)(int32_t))wglGetProcAddress("wglSwapIntervalEXT"))(bEnabled);
	}
	
	void WindowsWindow::SetVisible(bool bVisible)
	{
		mWindowData.Visible = bVisible;
		ShowWindow(mNativeHandle, bVisible);
	}
	
	void WindowsWindow::SetFocus(bool bEnabled)
	{
		mWindowData.Focused = bEnabled;
		if (bEnabled)
			::SetFocus(mNativeHandle);
		else
			::ShowWindow(mNativeHandle, SW_MINIMIZE);
	}
	
	void WindowsWindow::SetFullscreen(bool bEnabled)
	{
		mWindowData.Fullscreen = bEnabled;
		if (bEnabled)
		{
			auto [width, height] = GetWindowDimensions();

			GetWindowPlacement(mNativeHandle, &mPlacement.Placement);
			mPlacement.WindowWidth = width;
			mPlacement.WindowHeight = height;

			SetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtrW(mNativeHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(mNativeHandle, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

			ShowWindow(mNativeHandle, SW_MAXIMIZE);
			UpdateWindow(mNativeHandle);
		}
		else
		{
			SetWindowPlacement(mNativeHandle, &mPlacement.Placement);
			SetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE, WS_EX_OVERLAPPEDWINDOW);
			SetWindowLongPtrW(mNativeHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);

			ShowWindow(mNativeHandle, true);
			UpdateWindow(mNativeHandle);
		}
	}
	
	void WindowsWindow::SetResizable(bool bEnabled)
	{
		// TODO: implement
	}
	
	void WindowsWindow::ShowCursor()
	{
		mWindowData.CursorVisible = true;
		::SetCursor(mCursor);
		::ShowCursor(true);
	}
	
	void WindowsWindow::HideCursor()
	{
		mWindowData.CursorVisible = false;
		::SetCursor(NULL);
		::ShowCursor(false);
	}
	
	void WindowsWindow::Maximize()
	{
		mWindowData.Maximized = true;
		ShowWindow(mNativeHandle, SW_MAXIMIZE);
	}
	
	void WindowsWindow::CenterWindow()
	{
		mWindowData.Centered = true;

		RECT windowRect;
		GetWindowRect(mNativeHandle, &windowRect);

		int32_t xPos = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		int32_t yPos = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(mNativeHandle, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	
	void WindowsWindow::SetTitle(const std::string& title)
	{
		mWindowData.Title = title;
		SetWindowTextA(mNativeHandle, title.c_str());
	}

	bool WindowsWindow::IsVisible()
	{
		return mWindowData.Visible;
	}

	bool WindowsWindow::IsCursorHidden()
	{
		return !mWindowData.CursorVisible;
	}

	bool WindowsWindow::IsMaximized()
	{
		return mWindowData.Maximized;
	}

	bool WindowsWindow::IsFullscreen()
	{
		return mWindowData.Fullscreen;
	}
	
	bool WindowsWindow::IsCentered()
	{
		return mWindowData.Centered;
	}

	bool WindowsWindow::IsFocused()
	{
		return mWindowData.Focused;
	}

	const std::string& WindowsWindow::GetTitle()
	{
		return mWindowData.Title;
	}
	
	uint32_t WindowsWindow::GetWidth()
	{
		return mWindowData.Width;
	}

	uint32_t WindowsWindow::GetHeight()
	{
		return mWindowData.Height;
	}
	
	void* WindowsWindow::GetNativeHandle()
	{
		return (void*)mNativeHandle;
	}
	void* WindowsWindow::GetNativeContext()
	{
		return nullptr;
	}
	void* WindowsWindow::GetNativeCursor()
	{
		return nullptr;
	}

	void WindowsWindow::Init()
	{
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hInstance = NULL;
		wc.lpszClassName = L"ShaderSystemWindowClass";
		wc.lpszMenuName = L"";
		wc.lpfnWndProc = WindowProc;
		wc.style = NULL;

		if (!RegisterClassEx(&wc))
		{
			SHADER_SYSTEM_ERROR("Failed to register Win32 window class");
		}

		RECT window_rect;
		window_rect.left = 0;
		window_rect.top = 30;
		window_rect.right = window_rect.left + mWindowData.Width;
		window_rect.bottom = window_rect.top + mWindowData.Height;

		AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		mNativeHandle = CreateWindowExA(
			WS_EX_OVERLAPPEDWINDOW, "ShaderSystemWindowClass", mWindowData.Title.c_str(), WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
			window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
			NULL, NULL, NULL, &mWindowData
		);

		if (!mNativeHandle)
		{
			SHADER_SYSTEM_ERROR("Failed to create window HWND");
		}

		SetWindowLongPtr(mNativeHandle, GWLP_USERDATA, (DWORD_PTR)&mWindowData);

		if (mWindowData.Fullscreen)
		{
			auto WindowHDC = GetDC(mNativeHandle);
			auto FullscreenWidth = GetDeviceCaps(WindowHDC, DESKTOPHORZRES);
			auto FullscreenHeight = GetDeviceCaps(WindowHDC, DESKTOPVERTRES);
			auto ColourBits = GetDeviceCaps(WindowHDC, BITSPIXEL);
			auto RefreshRate = GetDeviceCaps(WindowHDC, VREFRESH);

			SetWindowLongPtr(mNativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtr(mNativeHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(mNativeHandle, HWND_TOPMOST, 0, 0, FullscreenWidth, FullscreenHeight, SWP_SHOWWINDOW);

			ShowWindow(mNativeHandle, SW_MAXIMIZE);
			UpdateWindow(mNativeHandle);

			mWindowData.Width = FullscreenWidth;
			mWindowData.Height = FullscreenHeight;
		}
		else
		{
			ShowWindow(mNativeHandle, SW_SHOW);
			UpdateWindow(mNativeHandle);
		}

		mContext = RenderingContext::Create(mNativeHandle);
		mContext->Init(mWindowData);
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CLOSE:
			{
				WindowData* data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					WindowCloseEvent e;
					if (data->EventCallback)
						data->EventCallback(e);
				}

				DestroyWindow(hwnd);
				break;
			}

			case WM_SIZE:
			{
				WindowData* data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					uint32_t width = (unsigned int)LOWORD(lParam);
					uint32_t height = (unsigned int)HIWORD(lParam);

					WindowResizeEvent e(width, height);
					if (data->EventCallback)
						data->EventCallback(e);

				}
				break;
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

#endif // SHADER_SYSTEM_PLATFORM_WINDOWS
