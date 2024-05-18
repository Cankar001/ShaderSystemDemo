#include "GLFWWindow.h"

#include "Core/Logger.h"
#include "Events/WindowEvent.h"

#include <GLFW/glfw3.h>

namespace ShaderSystem
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SHADER_SYSTEM_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GLFWWindow::GLFWWindow(const WindowData& properties)
		: mWindowData(properties)
	{
		Window::s_WindowInstance = this;
		Init();
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(mNativeHandle);
		glfwTerminate();
		s_GLFWInitialized = false;
	}

	void GLFWWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		mWindowData.EventCallback = callback;
	}
	
	const EventCallbackFn& GLFWWindow::GetEventCallback() const
	{
		return mWindowData.EventCallback;
	}
	
	void GLFWWindow::Update()
	{
		glfwPollEvents();
		
		glfwSwapBuffers(mNativeHandle);

		mContext->SwapBuffers();
	}
	
	void GLFWWindow::SetWindowIcon(const std::string& path, bool flip)
	{
	}
	
	std::pair<int32_t, int32_t> GLFWWindow::GetWindowDimensions()
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return { mode->width, mode->height };
	}
	
	std::pair<int32_t, int32_t> GLFWWindow::GetWindowPosition()
	{
		int32_t monitorX, monitorY;
		glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);
		return { monitorX, monitorY };
	}
	
	void GLFWWindow::CloseWindow()
	{
		WindowCloseEvent event;
		mWindowData.EventCallback(event);
		glfwDestroyWindow(mNativeHandle);
		glfwTerminate();
	}
	
	void GLFWWindow::SetVSync(bool bEnabled)
	{
		mWindowData.VSync = bEnabled;
		glfwSwapInterval(bEnabled);
	}
	
	void GLFWWindow::SetVisible(bool bVisible)
	{
		mWindowData.Visible = bVisible;
		if (bVisible)
			glfwShowWindow(mNativeHandle);
		else
			glfwHideWindow(mNativeHandle);
	}
	
	void GLFWWindow::SetFocus(bool bEnabled)
	{
		mWindowData.Focused = bEnabled;
		if (bEnabled)
			glfwFocusWindow(mNativeHandle);
		else
			glfwRestoreWindow(mNativeHandle);
	}
	
	void GLFWWindow::SetFullscreen(bool bEnabled)
	{
		mWindowData.Fullscreen = bEnabled;

		if (bEnabled)
		{
			// backup window position and window size
			glfwGetWindowPos(mNativeHandle, &mPlacement.WindowPosX, &mPlacement.WindowPosY);
			glfwGetWindowSize(mNativeHandle, &mPlacement.WindowWidth, &mPlacement.WindowHeight);

			// get resolution of monitor
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			// switch to full screen
			glfwSetWindowMonitor(mNativeHandle, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
		}
		else
		{
			// restore last window size and position
			glfwSetWindowMonitor(mNativeHandle, nullptr, mPlacement.WindowPosX, mPlacement.WindowPosY, mPlacement.WindowWidth, mPlacement.WindowHeight, 0);
		}
	}
	
	void GLFWWindow::SetResizable(bool bEnabled)
	{
		glfwSetWindowAttrib(mNativeHandle, GLFW_RESIZABLE, bEnabled ? GLFW_TRUE : GLFW_FALSE);
	}
	
	void GLFWWindow::ShowCursor()
	{
		mWindowData.CursorVisible = true;
		glfwSetInputMode(mNativeHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
	void GLFWWindow::HideCursor()
	{
		mWindowData.CursorVisible = false;
		glfwSetInputMode(mNativeHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	
	void GLFWWindow::Maximize()
	{
		mWindowData.Maximized = true;
		glfwMaximizeWindow(mNativeHandle);
	}
	
	void GLFWWindow::CenterWindow()
	{
		mWindowData.Centered = true;

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		int32_t monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int32_t width, height;
		glfwGetWindowSize(mNativeHandle, &width, &height);

		int32_t windowXPos = monitorX + (mode->width - width) / 2;
		int32_t windowYPos = monitorY + (mode->height - height) / 2;
		glfwSetWindowPos(mNativeHandle, windowXPos, windowYPos);
		mPlacement.WindowPosX = windowXPos;
		mPlacement.WindowPosY = windowYPos;
	}
	
	void GLFWWindow::SetTitle(const std::string& title)
	{
		mWindowData.Title = title;
		glfwSetWindowTitle(mNativeHandle, title.c_str());
	}
	
	bool GLFWWindow::IsVisible()
	{
		return mWindowData.Visible;
	}
	
	bool GLFWWindow::IsCursorHidden()
	{
		return !mWindowData.CursorVisible;
	}
	
	bool GLFWWindow::IsMaximized()
	{
		return mWindowData.Maximized;
	}
	
	bool GLFWWindow::IsFullscreen()
	{
		return mWindowData.Fullscreen;
	}
	
	bool GLFWWindow::IsCentered()
	{
		return mWindowData.Centered;
	}
	
	bool GLFWWindow::IsFocused()
	{
		return glfwGetWindowAttrib(mNativeHandle, GLFW_FOCUSED);
	}
	
	const std::string& GLFWWindow::GetTitle()
	{
		return mWindowData.Title;
	}
	
	uint32_t GLFWWindow::GetWidth()
	{
		return mWindowData.Width;
	}

	uint32_t GLFWWindow::GetHeight()
	{
		return mWindowData.Height;
	}
	
	void* GLFWWindow::GetNativeHandle()
	{
		return mNativeHandle;
	}
	
	void* GLFWWindow::GetNativeContext()
	{
		return nullptr;
	}
	
	void* GLFWWindow::GetNativeCursor()
	{
		return mNativeCursor;
	}

	float GLFWWindow::GetTime() const
	{
		return (float)glfwGetTime();
	}

	void GLFWWindow::Init()
	{
		uint32_t width = 0, height = 0;

		if (!s_GLFWInitialized)
		{
			glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		// In directX/Vulkan we need to disable the window wrapper to get the HWND window handle
#if defined(SHADER_SYSTEM_API_DX11) || defined(SHADER_SYSTEM_API_VULKAN)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif // SHADER_SYSTEM_API_DX11

		if (mWindowData.Fullscreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
			mNativeHandle = glfwCreateWindow(videoMode->width, videoMode->height, mWindowData.Title.c_str(), primaryMonitor, nullptr);
		}
		else
		{
			mNativeHandle = glfwCreateWindow((int32_t)mWindowData.Width, (int32_t)mWindowData.Height, mWindowData.Title.c_str(), nullptr, nullptr);
		}

		glfwSetCursor(mNativeHandle, mNativeCursor);

		mContext = RenderingContext::Create((void*)mNativeHandle);
		mContext->Init(mWindowData);

		glfwSetWindowUserPointer(mNativeHandle, &mWindowData);
	//	glfwSwapInterval(false);

		SetCallbacks();

		if (mWindowData.Centered)
		{
			CenterWindow();
		}
		else if (mWindowData.Maximized)
		{
			Maximize();
		}
	}

	void GLFWWindow::SetCallbacks()
	{
		glfwSetWindowSizeCallback(mNativeHandle, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent e(width, height);
			if (data.EventCallback)
				data.EventCallback(e);
		});

		glfwSetWindowCloseCallback(mNativeHandle, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			if (data.EventCallback)
				data.EventCallback(e);
		});

		glfwSetWindowFocusCallback(mNativeHandle, [](GLFWwindow* window, int focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Focused = (bool)focused;
		});
	}
}
