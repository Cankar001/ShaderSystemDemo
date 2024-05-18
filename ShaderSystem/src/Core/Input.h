#pragma once

#include "Core/Core.h"

namespace ShaderSystem
{
	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};

	class Input
	{
	public:

		static void Update();

		static bool IsKeyPressed(int32_t keyCode);
		static bool IsMouseButtonPressed(int32_t mouseButtonCode);

		static std::pair<double, double> GetMousePosition();
		static std::pair<double, double> GetAbsoluteMousePosition();

		static double GetMouseX();
		static double GetAbsoluteMouseX();

		static double GetMouseY();
		static double GetAbsoluteMouseY();

		static void SetCursorMode(CursorMode mode);
		static CursorMode GetCursorMode();

	};
}
