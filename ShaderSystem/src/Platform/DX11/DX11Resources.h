#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

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
	};
}
