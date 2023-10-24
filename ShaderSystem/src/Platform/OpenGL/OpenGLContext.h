#pragma once

#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	class OpenGLContext : public RenderingContext
	{
	public:

		OpenGLContext(void* inWindowHandle);
		virtual ~OpenGLContext();

		virtual void Init() override;
		virtual void MakeCurrent() override;

		virtual void* GetCurrentContext() override;

	private:

		void* mWindowHandle;
	};
}