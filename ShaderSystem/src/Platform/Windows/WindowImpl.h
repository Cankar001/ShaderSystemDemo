#pragma once

#include "Window/Window.h"

#if SHADER_SYSTEM_USE_NATIVE_WIN_API
#include "WindowsWindow.h"
#else
#include "GLFWWindow.h"
#endif
