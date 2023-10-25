#include "DX11Context.h"

#include "Core/Logger.h"

#include "Window/Window.h"

#if !SHADER_SYSTEM_USE_NATIVE_WIN_API
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

namespace ShaderSystem
{
	DX11Context::DX11Context(void *inWindowHandle)
	{
#if !SHADER_SYSTEM_USE_NATIVE_WIN_API
		mNativeWindowHandle = glfwGetWin32Window((GLFWwindow *)inWindowHandle);
		mWindowHandle = (GLFWwindow*)inWindowHandle;
#else
		mNativeWindowHandle = (HWND)inWindowHandle;
		mWindowHandle = (HWND)inWindowHandle;
#endif

		mBackbufferCount = 2;
		mBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		mDepthBufferFormat = DXGI_FORMAT_D16_UNORM;
	}

	DX11Context::~DX11Context()
	{
	}
	
	void DX11Context::Init(const WindowData &inData)
	{
		mWindowProperties = inData;

		CreateDeviceResources();
		::GetWindowRect(mNativeWindowHandle, &mOutputSize);
		CreateWindowDependentResources();

		InitializeDepthStencilState();
		InitializeViewPort();
		InitializeRasterizerState();
		InitializeBlendState();
		InitializeSamplerState();

#if 0
		InitializeSwapChain();
		InitializeRenderTargetView();
		InitializeDepthStencilViewAndBuffer();
#endif
	}
	
	void DX11Context::MakeCurrent()
	{
#if !SHADER_SYSTEM_USE_NATIVE_WIN_API
		glfwMakeContextCurrent((GLFWwindow *)mWindowHandle);
#endif
	}

	void DX11Context::SwapBuffers()
	{
		mSwapChain->Present(1, 0);
	//	DX11Resources::sDeviceContext->DiscardView(DX11Resources::sRenderTargetView.Get());
	//	if (DX11Resources::sDepthStencilView)
	//	{
	//		DX11Resources::sDeviceContext->DiscardView(DX11Resources::sDepthStencilView.Get());
	//	}
	}
	
	void *DX11Context::GetCurrentContext()
	{
#if !SHADER_SYSTEM_USE_NATIVE_WIN_API
		return (void *)glfwGetCurrentContext();
#else
		return nullptr;
#endif
	}

	void DX11Context::CreateDeviceResources()
	{
		HRESULT hr = E_FAIL;

		CreateFactory();

		ComPtr<IDXGIAdapter1> adapter;
		GetHardwareAdapter(adapter.GetAddressOf());

		// Create the Direct3D 11 API device object and a corresponding context.
		UINT device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef SHADER_SYSTEM_DEBUG
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		if (adapter)
		{
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			hr = D3D11CreateDevice(
				adapter.Get(),
				D3D_DRIVER_TYPE_UNKNOWN,
				nullptr,
				device_flags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				DX11Resources::sDevice.GetAddressOf(),  // Returns the Direct3D device created.
				&mFeatureLevel,        // Returns feature level of device created.
				DX11Resources::sDeviceContext.GetAddressOf()  // Returns the device immediate context.
			);
		}

#ifdef SHADER_SYSTEM_DEBUG
		ComPtr<ID3D11Debug> d3d_debug;
		if (SUCCEEDED(DX11Resources::sDevice.As(&d3d_debug)))
		{
			ComPtr<ID3D11InfoQueue> d3d_info_queue;
			if (SUCCEEDED(d3d_debug.As(&d3d_info_queue)))
			{
				d3d_info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3d_info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				};
				D3D11_INFO_QUEUE_FILTER filter = {};
				filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
				filter.DenyList.pIDList = hide;
				d3d_info_queue->AddStorageFilterEntries(&filter);
			}
		}
#endif

		SHADER_SYSTEM_INFO("Initialized DirectX 11.4 device and device context");

	//	hr = DX11Resources::sDeviceContext.As(&_annotation);
	//	if (FAILED(hr))
	//	{
	//		SHADER_SYSTEM_FATAL("Failed to create DX11 annotation: ({0})", hr);
	//		return;
	//	}
	}

	void DX11Context::CreateWindowDependentResources()
	{
		HRESULT hr;
		// Clear the previous window size specific context.
		ID3D11RenderTargetView *null_views[] = { nullptr };
		DX11Resources::sDeviceContext->OMSetRenderTargets(static_cast<UINT>(std::size(null_views)), null_views, nullptr);
		DX11Resources::sRenderTargetView.Reset();
		DX11Resources::sDepthStencilView.Reset();
		DX11Resources::sRenderTarget.Reset();
		DX11Resources::sDepthStencil.Reset();
		DX11Resources::sDeviceContext->Flush();

		// Determine the render target size in pixels.
		const UINT back_buffer_width = std::max<UINT>(static_cast<UINT>(mOutputSize.right - mOutputSize.left), 1u);
		const UINT back_buffer_height = std::max<UINT>(static_cast<UINT>(mOutputSize.bottom - mOutputSize.top), 1u);

		if (mSwapChain)
		{
			// If the swap chain already exists, resize it.
			hr = mSwapChain->ResizeBuffers(
				mBackbufferCount,
				back_buffer_width,
				back_buffer_height,
				mBackbufferFormat,
				0u
			);

			if (FAILED(hr))
			{
				SHADER_SYSTEM_FATAL("Could not resize swapchain: ({0})", hr);
				return;
			}
		}
		else
		{
			// Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapchain_desc{0};
			swapchain_desc.Width = back_buffer_width;
			swapchain_desc.Height = back_buffer_height;
			swapchain_desc.Format = mBackbufferFormat;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.BufferCount = mBackbufferCount;
			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			swapchain_desc.Scaling = DXGI_SCALING_STRETCH;
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
			swapchain_desc.Flags = 0;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fs_swapchain_desc{0};
			fs_swapchain_desc.Windowed = !mWindowProperties.Fullscreen;

			// Create a SwapChain from a Win32 window.
			hr = mFactory->CreateSwapChainForHwnd(DX11Resources::sDevice.Get(), mNativeWindowHandle, &swapchain_desc, &fs_swapchain_desc, nullptr, mSwapChain.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_FATAL("Could not create swapchain for HWND: ({0})", hr);
			}

			// This class does not support exclusive full-screen mode and prevents
			// DXGI from responding to the ALT+ENTER shortcut
			hr = mFactory->MakeWindowAssociation(mNativeWindowHandle, DXGI_MWA_NO_ALT_ENTER);
			if (FAILED(hr))
			{
				SHADER_SYSTEM_FATAL("Unable to make window association: ({0})", hr);
				return;
			}
		}

		// Create a render target view of the swap chain back buffer.
		hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(DX11Resources::sRenderTarget.ReleaseAndGetAddressOf()));
		if (FAILED(hr))
		{
			SHADER_SYSTEM_FATAL("Could not get back buffer from swapchain: ({0})", hr);
			return;
		}

		CD3D11_RENDER_TARGET_VIEW_DESC rt_view_desc(D3D11_RTV_DIMENSION_TEXTURE2D, mBackbufferFormat);
		hr = DX11Resources::sDevice->CreateRenderTargetView(
			DX11Resources::sRenderTarget.Get(),
			&rt_view_desc,
			DX11Resources::sRenderTargetView.ReleaseAndGetAddressOf()
		);

		if (FAILED(hr))
		{
			SHADER_SYSTEM_FATAL("Unable to create render target view: ({0})", hr);
			return;
		}

		if (mDepthBufferFormat != DXGI_FORMAT_UNKNOWN)
		{
			// Create a depth stencil view for use with 3D rendering if needed.
			CD3D11_TEXTURE2D_DESC depth_stencil_desc(
				mDepthBufferFormat,
				back_buffer_width,
				back_buffer_height,
				1, // This depth stencil view has only one texture.
				1, // Use a single mipmap level.
				D3D11_BIND_DEPTH_STENCIL
			);

			hr = DX11Resources::sDevice->CreateTexture2D(
				&depth_stencil_desc,
				nullptr,
				DX11Resources::sDepthStencil.ReleaseAndGetAddressOf()
			);

			if (FAILED(hr))
			{
				SHADER_SYSTEM_FATAL("Unable to create depth stencil texture: ({0})", hr);
				return;
			}

			CD3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Format = mDepthBufferFormat;
			dsv_desc.Flags = 0;
			dsv_desc.Texture2D.MipSlice = 0;
			hr = DX11Resources::sDevice->CreateDepthStencilView(
				DX11Resources::sDepthStencil.Get(),
				&dsv_desc,
				DX11Resources::sDepthStencilView.ReleaseAndGetAddressOf()
			);

			if (FAILED(hr))
			{
				SHADER_SYSTEM_FATAL("Failed to create depth stencil view: ({0})", hr);
				return;
			}
		}

		// Set the 3D rendering viewport to target the entire window.
		mViewport = CD3D11_VIEWPORT(
			0.0f,
			0.0f,
			static_cast<float>(back_buffer_width),
			static_cast<float>(back_buffer_height)
		);
	}

	void DX11Context::CreateFactory()
	{
		HRESULT hr = E_FAIL;
#ifdef SHADER_SYSTEM_DEBUG && (_WIN32_WINNT >= 0x0603 /*_WIN32_WINNT_WINBLUE*/)
		bool debugDXGI = false;
		{
			ComPtr<IDXGIInfoQueue> info_queue;
			if (SUCCEEDED(::DXGIGetDebugInterface1(0,
				IID_PPV_ARGS(info_queue.GetAddressOf()))))
			{
				debugDXGI = true;

				hr = ::CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(mFactory.ReleaseAndGetAddressOf()));

				if (FAILED(hr))
				{
					SHADER_SYSTEM_FATAL("CreateDXGIFactory2() failed: ({0})", hr);
					return;
				}

				info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
				info_queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

				DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
				{
					80 /* IDXGISwapChain::GetContainingOutput:
						  The swapchain's adapter does not control the output on
						  which the swapchain's window resides. */,
				};

				DXGI_INFO_QUEUE_FILTER filter{0};
				filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
				filter.DenyList.pIDList = hide;
				info_queue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
			}
		}

		if (!debugDXGI)
#endif
		{
			hr = ::CreateDXGIFactory2(0, IID_PPV_ARGS(mFactory.ReleaseAndGetAddressOf()));

			if (FAILED(hr))
			{
				SHADER_SYSTEM_FATAL("CreateDXGIFactory1() failed: ({0})", hr);
				return;
			}
		}
	}

	void DX11Context::GetHardwareAdapter(IDXGIAdapter1 **outAdapter)
	{
		HRESULT hr;

		*outAdapter = nullptr;

		ComPtr<IDXGIAdapter4> possible_adapter;
		ComPtr<IDXGIFactory6> factory6;

		hr = mFactory.As(&factory6);
		if (SUCCEEDED(hr))
		{
			for (UINT adapter_index = 0;
				 SUCCEEDED(factory6->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(possible_adapter.ReleaseAndGetAddressOf())));
				 adapter_index++)
			{
				DXGI_ADAPTER_DESC1 desc;
				possible_adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Don't select the Basic Render Driver adapter.
					continue;
				}

				break;
			}
		}

		if (!possible_adapter)
		{
			SHADER_SYSTEM_FATAL("Could not locate IDXGIAdapter4");
			return;
		}

		*outAdapter = possible_adapter.Detach();
	}

#if 0

	void DX11Context::InitializeSwapChain()
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapchainDesc.BufferCount = 1;
		swapchainDesc.BufferDesc.Width = mWindowProperties.Width;
		swapchainDesc.BufferDesc.Height = mWindowProperties.Height;
		swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.BufferDesc.RefreshRate.Numerator = 144;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.OutputWindow = mNativeWindowHandle;
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
		swapchainDesc.Windowed = !mWindowProperties.Fullscreen;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT result = D3D11CreateDeviceAndSwapChain(
			adapters[0].mAdapter, 
			D3D_DRIVER_TYPE_UNKNOWN, 
			NULL, NULL, NULL, 0, 
			D3D11_SDK_VERSION, 
			&swapchainDesc, 
			mSwapChain.GetAddressOf(), 
			DX11Resources::sDevice.GetAddressOf(), 
			NULL, 
			DX11Resources::sDeviceContext.GetAddressOf()
		);
		
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
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = mWindowProperties.Width;
		depthStencilDesc.Height = mWindowProperties.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		HRESULT result = DX11Resources::sDevice->CreateTexture2D(&depthStencilDesc, NULL, mDepthStencilBuffer.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create depth stencil buffer");

		result = DX11Resources::sDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, DX11Resources::sDepthStencilView.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create depth stencil view");
		else
			SHADER_SYSTEM_INFO("DX11 DepthStencilView and Buffer ready.");

		DX11Resources::sDeviceContext->OMSetRenderTargets(1, DX11Resources::sRenderTargetView.GetAddressOf(), DX11Resources::sDepthStencilView.Get());
	}
#endif
	
	void DX11Context::InitializeDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDesc.DepthEnable = true;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		HRESULT result = DX11Resources::sDevice->CreateDepthStencilState(&depthStencilStateDesc, DX11Resources::sDepthStencilState.GetAddressOf());
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
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		HRESULT result = DX11Resources::sDevice->CreateRasterizerState(&rasterizerDesc, DX11Resources::sRasterizerState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create rasterizer state");
		else
			SHADER_SYSTEM_INFO("DX11 RasterizerState ready.");
	}
	
	void DX11Context::InitializeBlendState()
	{
		D3D11_BLEND_DESC omDesc;
		ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
		omDesc.RenderTarget[0].BlendEnable = true;
		omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT result = DX11Resources::sDevice->CreateBlendState(&omDesc, mBlendState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create blend state");
		else
			SHADER_SYSTEM_INFO("DX11 BlendState ready.");

		DX11Resources::sDeviceContext->OMSetBlendState(mBlendState.Get(), 0, 0xffffffff);
	}
	
	void DX11Context::InitializeSamplerState()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT result = DX11Resources::sDevice->CreateSamplerState(&samplerDesc, DX11Resources::sSamplerState.GetAddressOf());
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create sampler state");
		else
			SHADER_SYSTEM_INFO("DX11 Sampler state ready.");
	}
}
