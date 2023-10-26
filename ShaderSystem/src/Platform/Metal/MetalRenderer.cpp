#include "MetalRenderer.h"

namespace ShaderSystem
{
	MetalRenderer::MetalRenderer(RenderingAPIType inType)
		: RendererAPI(inType)
	{
	}

	MetalRenderer::~MetalRenderer()
	{
	}
	
	void MetalRenderer::Init()
	{
	}
	
	void MetalRenderer::Shutdown()
	{
	}
	
	void MetalRenderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
	}
	
	void MetalRenderer::EndFrame(uint32_t inIndexCount)
	{
	}
}
