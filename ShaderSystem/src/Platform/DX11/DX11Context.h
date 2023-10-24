#pragma once

#include "Core/Core.h"
#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	class DX11Context : public RenderingContext
	{
	public:

		DX11Context(void *inWindowHandle);
		virtual ~DX11Context();

		virtual void Init() override;
		virtual void MakeCurrent() override;

		virtual void *GetCurrentContext() override;

	private:

		void *mWindowHandle;
	};
}
