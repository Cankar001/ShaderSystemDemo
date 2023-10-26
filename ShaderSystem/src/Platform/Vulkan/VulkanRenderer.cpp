#include "VulkanRenderer.h"

namespace ShaderSystem
{
	VulkanRenderer::VulkanRenderer(RenderingAPIType inType)
		: RendererAPI(inType)
	{
	}

	VulkanRenderer::~VulkanRenderer()
	{
	}
	
	void VulkanRenderer::Init()
	{
	}
	
	void VulkanRenderer::Shutdown()
	{
	}
	
	void VulkanRenderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
	}
	
	void VulkanRenderer::EndFrame(uint32_t inIndexCount)
	{
	}
}
