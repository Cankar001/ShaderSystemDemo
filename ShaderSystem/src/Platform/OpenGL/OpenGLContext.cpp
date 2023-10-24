#include "OpenGLContext.h"

#include "Core/Logger.h"
#include "Window/Window.h"

#include <glad/glad.h>

#if !SHADER_SYSTEM_USE_NATIVE_WIN_API

#include <GLFW/glfw3.h>

namespace ShaderSystem
{
	OpenGLContext::OpenGLContext(void* inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}
	
	void OpenGLContext::Init(const WindowData &inData)
	{
		glfwMakeContextCurrent((GLFWwindow*)mWindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			SHADER_SYSTEM_FATAL("Failed to load Context!");
	}
	
	void OpenGLContext::MakeCurrent()
	{
		glfwMakeContextCurrent((GLFWwindow*)mWindowHandle);
	}
	
	void* OpenGLContext::GetCurrentContext()
	{
		return (void*)glfwGetCurrentContext();
	}
}
#else

#ifdef SHADER_SYSTEM_PLATFORM_WINDOWS

#include <Windows.h>

namespace ShaderSystem
{
	static HDC dc = nullptr;
	static HGLRC rc = nullptr;

	OpenGLContext::OpenGLContext(void* inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
		ReleaseDC((HWND)mWindowHandle, dc);
		wglDeleteContext(rc);
	}

	void OpenGLContext::Init(const WindowData &inData)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1,							   // version number
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
			24,							   // 24-bit color depth
			0, 0, 0, 0, 0, 0,			   // color bits ignored
			0,							   // no alpha buffer
			0,							   // shift bit ignored
			0,							   // no accumulation buffer
			0, 0, 0, 0,					   // accum bits ignored
			32,							   // 32-bit z-buffer
			0,							   // no stencil buffer
			0,							   // no auxiliary buffer
			PFD_MAIN_PLANE,				   // main layer
			0,							   // reserved
			0, 0, 0						   // layer masks ignored
		};

		dc = GetDC((HWND)mWindowHandle);
		SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
		rc = wglCreateContext(dc);

		wglMakeCurrent(dc, rc);
		if (!gladLoadGL())
			SHADER_SYSTEM_FATAL("Failed to load Context!");

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLContext::MakeCurrent()
	{
		wglMakeCurrent(dc, rc);
	}

	void* OpenGLContext::GetCurrentContext()
	{
		return (void*)dc;
	}
}

#else
#error "Platform not supported."
#endif // SHADER_SYSTEM_PLATFORM_WINDOWS

#endif // SHADER_SYSTEM_USE_NATIVE_WIN_API
