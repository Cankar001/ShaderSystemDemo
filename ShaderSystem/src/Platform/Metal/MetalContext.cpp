#include "MetalContext.h"

namespace ShaderSystem
{
	MetalContext::MetalContext(void *inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}
	
	MetalContext::~MetalContext()
	{
	}
	
	void MetalContext::Init(const WindowData &inData)
	{
	}
	
	void MetalContext::MakeCurrent()
	{
	}
	
	void MetalContext::SwapBuffers()
	{
	}
	
	void *MetalContext::GetCurrentContext()
	{
		return nullptr;
	}
}
