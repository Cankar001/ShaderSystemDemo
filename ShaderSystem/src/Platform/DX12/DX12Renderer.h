#pragma once

#include "Core/Core.h"

#include "Interfaces/RendererAPI.h"

namespace ShaderSystem
{
	class DX12Renderer : public RendererAPI
	{
	public:

		DX12Renderer(RenderingAPIType inType);
		virtual ~DX12Renderer();

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor) override;
		virtual void EndFrame(uint32_t inIndexCount) override;

		virtual std::string ToString() override { return "DirectX12"; };
		virtual RenderingAPIType GetType() const override { return RenderingAPIType::DirectX12; }
	};
}
