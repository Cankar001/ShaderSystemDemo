#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>

// TODO: integrate these in premake instead.
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace ShaderSystem
{
	class DX12Resources
	{
	public:

		static ComPtr<ID3D12Device>				sDevice;
	//	static ComPtr<ID3D12DeviceContext>		sDeviceContext;
	//	static ComPtr<ID3D12RenderTargetView>	sRenderTargetView;
	//	static ComPtr<ID3D12RasterizerState>	sRasterizerState;
	//	static ComPtr<ID3D12DepthStencilState>	sDepthStencilState;
	//	static ComPtr<ID3D12DepthStencilView>	sDepthStencilView;
	//	static ComPtr<ID3D12SamplerState>		sSamplerState;
	};
}
