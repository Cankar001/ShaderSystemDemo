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

	void Renderer::SelectRenderingBackend(RenderingAPIType inType)
	{
		sGPURenderer = RendererAPI::Create(inType);
	}

	void Renderer::Init()
	{
		s_Data = new RenderingData();
		s_Data->Library = MakeRef<ShaderLibrary>();

		ShaderCache::Init();

		BufferLayout flatColorShaderLayout = {
			BufferElement("a_Position", ShaderDataType::Float3, false),
			BufferElement("a_TexCoord", ShaderDataType::Float2, false),
			BufferElement("a_Color", ShaderDataType::Float4, false)
		};

		s_Data->Library->Load("assets/shaders/FlatColorShader.glsl", flatColorShaderLayout, true);
	}

	void Renderer::Shutdown()
	{
		if (s_Data)
		{
			s_Data->Library.reset();

			ShaderCache::Shutdown();

			sGPURenderer = nullptr;

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

	RenderingAPIType Renderer::GetCurrentRenderingAPIType()
	{
		return sGPURenderer->GetType();
	}

	Ref<RendererAPI> Renderer::GetCurrentRenderingAPI()
	{
		return sGPURenderer;
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
