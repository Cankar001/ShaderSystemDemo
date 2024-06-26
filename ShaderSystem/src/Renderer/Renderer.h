#pragma once

#include "Core/Core.h"

#include "Graphics/ShaderLibrary.h"
#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class Renderer
	{
	public:

		static void SelectRenderingBackend(RenderingAPIType inType);

		static void Init();
		static void Shutdown();

		static void ChangeRendererBackend(RenderingAPIType inType);

		static void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor);
		static void EndFrame(uint32_t inIndexCount);

		static void OnShaderReloaded(uint64_t inHash);
		static RenderingAPIType GetCurrentRenderingAPIType();
		static Ref<RendererAPI> GetCurrentRenderingAPI();

		static Ref<ShaderLibrary> GetShaderLibrary();

	private:

		static Ref<RendererAPI> sGPURenderer;
	};
}

