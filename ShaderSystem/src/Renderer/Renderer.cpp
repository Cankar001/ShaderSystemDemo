#include "Renderer.h"

#include "Graphics/ShaderCache.h"

namespace ShaderSystem
{
	struct RenderingData
	{
		Ref<ShaderLibrary> Library;
	};

	Ref<RendererAPI> Renderer::sGPURenderer;
	static RenderingData *s_Data;

	void Renderer::Init()
	{
		s_Data = new RenderingData();
		s_Data->Library = MakeRef<ShaderLibrary>();

		ShaderCache::Init();

		s_Data->Library->Load("assets/shaders/FlatColorShader.glsl", true);

		sGPURenderer = RendererAPI::Create();
	}

	void Renderer::Shutdown()
	{
		sGPURenderer = nullptr;

		if (s_Data)
		{
			s_Data->Library.reset();

			ShaderCache::Shutdown();

			delete s_Data;
			s_Data = nullptr;
		}
	}

	void Renderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
		sGPURenderer->BeginFrame(inWindowWidth, inWindowHeight, inClearColor);
	}

	void Renderer::EndFrame(uint32_t inIndexCount)
	{
		sGPURenderer->EndFrame(inIndexCount);
	}

	void Renderer::OnShaderReloaded(uint64_t inHash)
	{
		// TODO: if we introduce shader dependencies, we can reload them here
	}

	Ref<ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return s_Data->Library;
	}
}
