#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_6.h>

#ifdef SHADER_SYSTEM_DEBUG
#include <dxgidebug.h>
#endif

// TODO: integrate these in premake instead.
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace ShaderSystem
{
	class DX11Resources
	{
	public:

		static ComPtr<ID3D11Device>				sDevice;
		static ComPtr<ID3D11DeviceContext>		sDeviceContext;
		static ComPtr<ID3D11RenderTargetView>	sRenderTargetView;
		static ComPtr<ID3D11RasterizerState>	sRasterizerState;
		static ComPtr<ID3D11DepthStencilState>	sDepthStencilState;
		static ComPtr<ID3D11DepthStencilView>	sDepthStencilView;
		static ComPtr<ID3D11SamplerState>		sSamplerState;
		static ComPtr<ID3D11Texture2D>			sRenderTarget;
		static ComPtr<ID3D11Texture2D>			sDepthStencil;
	};
}
