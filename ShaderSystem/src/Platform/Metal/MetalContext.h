#pragma once

#include "Core/Core.h"
#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	class MetalContext : public RenderingContext
	{
	public:

		MetalContext(void *inWindowHandle);
		virtual ~MetalContext();

		virtual void Init(const WindowData &inData) override;
		virtual void MakeCurrent() override;

		virtual void SwapBuffers() override;

		virtual void *GetCurrentContext() override;

	private:

		void *mWindowHandle;
	};
}
