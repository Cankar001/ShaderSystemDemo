#pragma once

#include "Core/Core.h"

namespace ShaderSystem
{
	class RenderingContext
	{
	public:

		virtual ~RenderingContext() {}

		virtual void Init() = 0;
		virtual void MakeCurrent() = 0;

		virtual void* GetCurrentContext() = 0;

		static Ref<RenderingContext> Create(void* inWindowHandle);
	};
}
