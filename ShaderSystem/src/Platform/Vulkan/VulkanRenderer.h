#pragma once

#include "Core/Core.h"
#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class VulkanRenderer : public RendererAPI
	{
	public:

		VulkanRenderer(RenderingAPIType inType);
		virtual ~VulkanRenderer();

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) override;
		virtual void EndFrame(uint32_t inIndexCount) override;

		virtual std::string ToString() override { return "Vulkan"; }
		virtual RenderingAPIType GetType() const override { return RenderingAPIType::Vulkan; }

	private:


	};
}
