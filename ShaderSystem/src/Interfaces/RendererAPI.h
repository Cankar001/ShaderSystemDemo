#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>
#include <string>

namespace ShaderSystem
{
	enum class RenderingAPIType
	{
		OpenGL = 0,
		DirectX11,
		DirectX12,
		Metal,
		Vulkan
	};

	class RendererAPI
	{
	public:

		virtual ~RendererAPI() {}

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) = 0;
		virtual void EndFrame(uint32_t inIndexCount) = 0;

		virtual std::string ToString() = 0;
		virtual RenderingAPIType GetType() const = 0;

		static Ref<RendererAPI> Create(RenderingAPIType inType);

	protected:

		RendererAPI(RenderingAPIType inType);

	protected:

		RenderingAPIType mType;
	};
}
