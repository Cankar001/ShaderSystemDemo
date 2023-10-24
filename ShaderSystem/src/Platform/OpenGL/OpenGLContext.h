#pragma once

#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	class OpenGLContext : public RenderingContext
	{
	public:

		OpenGLContext(void* inWindowHandle);
		virtual ~OpenGLContext();

		virtual void Init(const WindowData &inData) override;
		virtual void MakeCurrent() override;

		virtual void* GetCurrentContext() override;

	private:

		void* mWindowHandle;
	};
}
