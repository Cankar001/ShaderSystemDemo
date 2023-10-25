#include "DX12Context.h"

namespace ShaderSystem
{
	DX12Context::DX12Context(void *inWindowHandle)
		: mWindowHandle(inWindowHandle)
	{
	}

	DX12Context::~DX12Context()
	{
	}

	void DX12Context::Init(const WindowData &inData)
	{
	}

	void DX12Context::MakeCurrent()
	{
	}
	
	void DX12Context::SwapBuffers()
	{
	}
	
	void *DX12Context::GetCurrentContext()
	{
		return nullptr;
	}
}
