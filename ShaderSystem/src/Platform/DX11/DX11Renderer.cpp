#include "DX11Renderer.h"

namespace ShaderSystem
{
	DX11Renderer::DX11Renderer(RenderingAPIType inType)
		: RendererAPI(inType)
	{
	}

	DX11Renderer::~DX11Renderer()
	{
	}
	
	void DX11Renderer::Init()
	{
	}
	
	void DX11Renderer::Shutdown()
	{
	}
	
	void DX11Renderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
	}
	
	void DX11Renderer::EndFrame(uint32_t inIndexCount)
	{
	}
}
