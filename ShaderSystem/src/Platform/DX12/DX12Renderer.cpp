#include "DX12Renderer.h"

namespace ShaderSystem
{
	DX12Renderer::DX12Renderer(RenderingAPIType inType)
		: RendererAPI(inType)
	{
	}
	
	DX12Renderer::~DX12Renderer()
	{
	}
	
	void DX12Renderer::Init()
	{
	}
	
	void DX12Renderer::Shutdown()
	{
	}
	
	void DX12Renderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
	}
	
	void DX12Renderer::EndFrame(uint32_t inIndexCount)
	{
	}
}
