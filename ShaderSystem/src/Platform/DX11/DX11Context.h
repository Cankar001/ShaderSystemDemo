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

		virtual void SwapBuffers() override;

		virtual void *GetCurrentContext() override;

	private:

		void CreateDeviceResources();
		void CreateWindowDependentResources();
		void CreateFactory();
		void GetHardwareAdapter(IDXGIAdapter1 **outAdapter);

#if 0
		void InitializeSwapChain();
		void InitializeRenderTargetView();
		void InitializeDepthStencilViewAndBuffer();
#endif

		void InitializeDepthStencilState();
		void InitializeViewPort();
		void InitializeRasterizerState();
		void InitializeBlendState();
		void InitializeSamplerState();

	private:

		void *mWindowHandle;
		HWND mNativeWindowHandle;
		WindowData mWindowProperties;

		ComPtr<IDXGISwapChain1>	 mSwapChain;
		ComPtr<ID3D11Texture2D>	 mDepthStencilBuffer;
		ComPtr<ID3D11BlendState> mBlendState;
		ComPtr<IDXGIFactory2>	 mFactory;

		RECT mOutputSize;
		CD3D11_VIEWPORT mViewport;
		D3D_FEATURE_LEVEL mFeatureLevel;
		UINT mBackbufferCount;
		DXGI_FORMAT mBackbufferFormat;
		DXGI_FORMAT mDepthBufferFormat;
	};
}
