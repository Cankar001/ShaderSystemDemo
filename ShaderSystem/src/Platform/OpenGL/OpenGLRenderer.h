#pragma once

#include "Core/Core.h"

#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class OpenGLRenderer : public RendererAPI
	{
	public:

		OpenGLRenderer();
		virtual ~OpenGLRenderer();

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) override;
		virtual void EndFrame(uint32_t inIndexCount) override;
	};
}
