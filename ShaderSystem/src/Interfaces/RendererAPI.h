#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace ShaderSystem
{
	class RendererAPI
	{
	public:

		virtual ~RendererAPI() {}

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) = 0;
		virtual void EndFrame(uint32_t inIndexCount) = 0;

		static Ref<RendererAPI> Create();
	};
}
