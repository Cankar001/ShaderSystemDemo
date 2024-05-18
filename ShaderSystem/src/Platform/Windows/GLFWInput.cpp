#include "Core/Input.h"

#include "Window/Window.h"

#include <GLFW/glfw3.h>

namespace ShaderSystem
{
	void Input::Update()
	{
		// NOTE: Not relevant for keyboard input.
	}

	bool Input::IsKeyPressed(int32_t keyCode)
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Window::Get().GetNativeHandle());
		int32_t state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int32_t mouseButtonCode)
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Window::Get().GetNativeHandle());
		int32_t state = glfwGetMouseButton(window, mouseButtonCode);
		return state == GLFW_PRESS;
	}

	std::pair<double, double> Input::GetMousePosition()
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Window::Get().GetNativeHandle());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		uint32_t width = Window::Get().GetWidth();
		uint32_t height = Window::Get().GetHeight();

		if (xPos > width)
			xPos = width;
		else if (xPos < 0.0)
			xPos = 0.0;

		if (yPos > height)
			yPos = height;
		else if (yPos < 0.0)
			yPos = 0.0;

		return { xPos, yPos };
	}

	std::pair<double, double> Input::GetAbsoluteMousePosition()
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Window::Get().GetNativeHandle());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { xPos, yPos };
	}

	double Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	double Input::GetAbsoluteMouseX()
	{
		auto [x, y] = GetAbsoluteMousePosition();
		return x;
	}

	double Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

	double Input::GetAbsoluteMouseY()
	{
		auto [x, y] = GetAbsoluteMousePosition();
		return y;
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Window::Get().GetNativeHandle());
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int32_t)mode);
	}
	
	CursorMode Input::GetCursorMode()
	{
		GLFWwindow *window = static_cast<GLFWwindow *>(Window::Get().GetNativeHandle());
		return (CursorMode)(glfwGetInputMode(window, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
	}
}
