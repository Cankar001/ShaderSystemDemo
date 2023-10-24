#include "DX11Resources.h"

namespace ShaderSystem
{
	ComPtr<ID3D11Device>			DX11Resources::sDevice = nullptr;
	ComPtr<ID3D11DeviceContext>		DX11Resources::sDeviceContext = nullptr;
	ComPtr<ID3D11RenderTargetView>	DX11Resources::sRenderTargetView = nullptr;
	ComPtr<ID3D11RasterizerState>	DX11Resources::sRasterizerState = nullptr;
	ComPtr<ID3D11DepthStencilState> DX11Resources::sDepthStencilState = nullptr;
	ComPtr<ID3D11DepthStencilView>	DX11Resources::sDepthStencilView = nullptr;
	ComPtr<ID3D11SamplerState>		DX11Resources::sSamplerState = nullptr;
}
