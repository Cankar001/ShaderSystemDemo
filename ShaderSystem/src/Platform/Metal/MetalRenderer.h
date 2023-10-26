#pragma once

#include "Core/Core.h"
#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class MetalRenderer : public RendererAPI
	{
	public:

		MetalRenderer(RenderingAPIType inType);
		virtual ~MetalRenderer();

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) override;
		virtual void EndFrame(uint32_t inIndexCount) override;

		virtual std::string ToString() override { return "Metal"; }
		virtual RenderingAPIType GetType() const override { return RenderingAPIType::Metal; }

	private:


	};
}
