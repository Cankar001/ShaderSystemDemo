#pragma once

#include "Core/Core.h"

#include "Graphics/ShaderLibrary.h"

namespace ShaderSystem
{
	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void OnShaderReloaded(uint64_t inHash);

		static Ref<ShaderLibrary> GetShaderLibrary();
	};
}

