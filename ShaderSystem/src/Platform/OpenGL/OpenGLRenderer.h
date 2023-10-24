#pragma once

#include "Core/Core.h"

#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class OpenGLRenderer : public RendererAPI
	{
	public:

		OpenGLRenderer(RenderingAPIType inType);
		virtual ~OpenGLRenderer();

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) override;
		virtual void EndFrame(uint32_t inIndexCount) override;

		virtual RenderingAPIType GetType() const override { return RenderingAPIType::OpenGL; }
		virtual std::string ToString() override { return "OpenGL"; }
	};
}
