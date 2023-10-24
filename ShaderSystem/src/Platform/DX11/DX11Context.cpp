#include "DX11Context.h"

#include "Core/Logger.h"

#include "Window/Window.h"

#if !SHADER_SYSTEM_USE_NATIVE_WIN_API
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include "DX11AdapterReader.h"

namespace ShaderSystem
{
	DX11Context::DX11Context(void *inWindowHandle)
	{
#if !SHADER_SYSTEM_USE_NATIVE_WIN_API
		mWindowHandle = glfwGetWin32Window((GLFWwindow *)inWindowHandle);
#else
		mWindowHandle = (HWND)inWindowHandle;
#endif
	}

	DX11Context::~DX11Context()
	{
	}
	
	void DX11Context::Init(const WindowData &inData)
	{
		mWindowProperties = inData;

		InitializeSwapChain();
		InitializeRenderTargetView();
		InitializeDepthStencilViewAndBuffer();
		InitializeDepthStencilState();
		InitializeViewPort();
		InitializeRasterizerState();
		InitializeBlendState();
		InitializeSamplerState();
	}
	
	void DX11Context::MakeCurrent()
	{
	}
	
	void *DX11Context::GetCurrentContext()
	{
		return nullptr;
	}

	void DX11Context::InitializeSwapChain()
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		DXGI_SWAP_CHAIN_DESC swapchaindesc;
		ZeroMemory(&swapchaindesc, sizeof(swapchaindesc));

		swapchaindesc.BufferCount = 1;
		swapchaindesc.BufferDesc.Width = mWindowProperties.Width;
		swapchaindesc.BufferDesc.Height = mWindowProperties.Height;
		swapchaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchaindesc.BufferDesc.RefreshRate.Numerator = 144;
		swapchaindesc.BufferDesc.RefreshRate.Denominator = 1;
		swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchaindesc.OutputWindow = mWindowHandle;
		swapchaindesc.SampleDesc.Count = 1;
		swapchaindesc.SampleDesc.Quality = 0;
		swapchaindesc.Windowed = !mWindowProperties.Fullscreen;
		swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapchaindesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT result = D3D11CreateDeviceAndSwapChain(adapters[0].mAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, NULL, 0, D3D11_SDK_VERSION, &swapchaindesc, mSwapChain.GetAddressOf(), DX11Resources::sDevice.GetAddressOf(), NULL, DX11Resources::sDeviceContext.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create ID3D11Device and IDXGISwapChain");
		else
			SHADER_SYSTEM_INFO("DX11 Swapchain ready.");
	}
	
	void DX11Context::InitializeRenderTargetView()
	{
		ComPtr<ID3D11Texture2D> back_buffer;
		HRESULT result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(back_buffer.GetAddressOf()));
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to get back buffer");

		result = DX11Resources::sDevice->CreateRenderTargetView(back_buffer.Get(), NULL, DX11Resources::sRenderTargetView.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create RenderTargetView");
		else
			SHADER_SYSTEM_INFO("DX11 RenderTargetView ready.");
	}
	
	void DX11Context::InitializeDepthStencilViewAndBuffer()
	{
		D3D11_TEXTURE2D_DESC depth_stencil_desc;
		depth_stencil_desc.Width = mWindowProperties.Width;
		depth_stencil_desc.Height = mWindowProperties.Height;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
		depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_desc.CPUAccessFlags = 0;
		depth_stencil_desc.MiscFlags = 0;

		HRESULT result = DX11Resources::sDevice->CreateTexture2D(&depth_stencil_desc, NULL, mDepthStencilBuffer.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create depth stencil buffer");

		result = DX11Resources::sDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, DX11Resources::sDepthStencilView.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create depth stencil view");
		else
			SHADER_SYSTEM_INFO("DX11 DepthStencilView and Buffer ready.");

		DX11Resources::sDeviceContext->OMSetRenderTargets(1, DX11Resources::sRenderTargetView.GetAddressOf(), DX11Resources::sDepthStencilView.Get());
	}
	
	void DX11Context::InitializeDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc;
		ZeroMemory(&depth_stencil_state_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depth_stencil_state_desc.DepthEnable = true;
		depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_state_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		HRESULT result = DX11Resources::sDevice->CreateDepthStencilState(&depth_stencil_state_desc, DX11Resources::sDepthStencilState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create depth stencil state");
		else
			SHADER_SYSTEM_INFO("DX11 DepthStencilState ready.");
	}
	
	void DX11Context::InitializeViewPort()
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		viewport.Width = (FLOAT)mWindowProperties.Width;
		viewport.Height = (FLOAT)mWindowProperties.Height;

		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DX11Resources::sDeviceContext->RSSetViewports(1, &viewport);
		SHADER_SYSTEM_INFO("DX11 Viewport ready.");
	}
	
	void DX11Context::InitializeRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc;
		ZeroMemory(&rasterizerdesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerdesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerdesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		HRESULT result = DX11Resources::sDevice->CreateRasterizerState(&rasterizerdesc, DX11Resources::sRasterizerState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create rasterizer state");
		else
			SHADER_SYSTEM_INFO("DX11 RasterizerState ready.");
	}
	
	void DX11Context::InitializeBlendState()
	{
		D3D11_BLEND_DESC om_desc;
		ZeroMemory(&om_desc, sizeof(D3D11_BLEND_DESC));
		om_desc.RenderTarget[0].BlendEnable = true;
		om_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		om_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		om_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		om_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		om_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		om_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		om_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT result = DX11Resources::sDevice->CreateBlendState(&om_desc, mBlendState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create blend state");
		else
			SHADER_SYSTEM_INFO("DX11 BlendState ready.");

		DX11Resources::sDeviceContext->OMSetBlendState(mBlendState.Get(), 0, 0xffffffff);
	}
	
	void DX11Context::InitializeSamplerState()
	{
		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(D3D11_SAMPLER_DESC));

		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MaxAnisotropy = 0;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11Resources::sDevice->CreateSamplerState(&sampler_desc, DX11Resources::sSamplerState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create sampler state");
		else
			SHADER_SYSTEM_INFO("DX11 Sampler state ready.");
	}
}
