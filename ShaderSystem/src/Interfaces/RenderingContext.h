#pragma once

#include "Core/Core.h"
#include "Window/Window.h"

namespace ShaderSystem
{
	class RenderingContext
	{
	public:

		virtual ~RenderingContext() {}

		virtual void Init(const WindowData &inData) = 0;
		virtual void MakeCurrent() = 0;

		virtual void* GetCurrentContext() = 0;

		static Ref<RenderingContext> Create(void* inWindowHandle);
	};
}
