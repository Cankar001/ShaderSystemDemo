#include "VulkanContext.h"

namespace ShaderSystem
{
	VulkanContext::VulkanContext(void *inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}

	VulkanContext::~VulkanContext()
	{
	}
	
	void VulkanContext::Init(const WindowData &inData)
	{
	}
	
	void VulkanContext::MakeCurrent()
	{
	}
	
	void VulkanContext::SwapBuffers()
	{
	}
	
	void *VulkanContext::GetCurrentContext()
	{
		return nullptr;
	}
}
