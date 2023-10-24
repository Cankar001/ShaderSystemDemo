#pragma once

#include "Core/Core.h"
#include "Interfaces/RenderingContext.h"

#include "DX11Resources.h"

namespace ShaderSystem
{
	class DX11Context : public RenderingContext
	{
	public:

		DX11Context(void *inWindowHandle);
		virtual ~DX11Context();

		virtual void Init(const WindowData &inData) override;
		virtual void MakeCurrent() override;

		virtual void *GetCurrentContext() override;

	private:

		void InitializeSwapChain();
		void InitializeRenderTargetView();
		void InitializeDepthStencilViewAndBuffer();
		void InitializeDepthStencilState();
		void InitializeViewPort();
		void InitializeRasterizerState();
		void InitializeBlendState();
		void InitializeSamplerState();

	private:

		HWND mWindowHandle;
		WindowData mWindowProperties;

		ComPtr<IDXGISwapChain>	 mSwapChain;
		ComPtr<ID3D11Texture2D>	 mDepthStencilBuffer;
		ComPtr<ID3D11BlendState> mBlendState;
	};
}
