#include "Renderer.h"

#include "Shader/ShaderCache.h"

namespace ShaderSystem
{
	struct RenderingData
	{
		Ref<ShaderLibrary> Library;
	};

	static RenderingData *s_Data;

	void Renderer::Init()
	{
		s_Data = new RenderingData();
		s_Data->Library = MakeRef<ShaderLibrary>();

		ShaderCache::Init();

		s_Data->Library->Load("assets/shaders/FlatColorShader.glsl", true);
	}

	void Renderer::Shutdown()
	{
		if (s_Data)
		{
			s_Data->Library.reset();

			ShaderCache::Shutdown();

			delete s_Data;
			s_Data = nullptr;
		}
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
