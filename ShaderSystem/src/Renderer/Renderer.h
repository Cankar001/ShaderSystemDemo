#pragma once

#include "Core/Core.h"

#include "Graphics/ShaderLibrary.h"
#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor);
		static void EndFrame(uint32_t inIndexCount);

		static void OnShaderReloaded(uint64_t inHash);

		static Ref<ShaderLibrary> GetShaderLibrary();

	private:

		static Ref<RendererAPI> sGPURenderer;
	};
}

