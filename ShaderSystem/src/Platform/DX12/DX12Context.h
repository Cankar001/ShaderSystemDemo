#pragma once

#include "Core/Core.h"
#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	class DX12Context : public RenderingContext
	{
	public:

		DX12Context(void *inWindowHandle);
		virtual ~DX12Context();

		virtual void Init(const WindowData &inData) override;
		virtual void MakeCurrent() override;

		virtual void SwapBuffers() override;

		virtual void *GetCurrentContext() override;

	private:

		void *mWindowHandle;
		WindowData mWindowProperties;
	};
}
