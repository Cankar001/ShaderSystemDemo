#pragma once

#include "Core/Core.h"
#include "Interfaces/RenderingContext.h"

namespace ShaderSystem
{
	class VulkanContext : public RenderingContext
	{
	public:

		VulkanContext(void *inWindowHandle);
		virtual ~VulkanContext();

		virtual void Init(const WindowData &inData) override;
		virtual void MakeCurrent() override;

		virtual void SwapBuffers() override;

		virtual void *GetCurrentContext() override;

	private:

		void *mWindowHandle;
	};
}
