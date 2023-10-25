#include "DX11Renderer.h"

#include "DX11Resources.h"

namespace ShaderSystem
{
	DX11Renderer::DX11Renderer(RenderingAPIType inType)
		: RendererAPI(inType)
	{
	}

	DX11Renderer::~DX11Renderer()
	{
	}
	
	void DX11Renderer::Init()
	{
	}
	
	void DX11Renderer::Shutdown()
	{
	}
	
	void DX11Renderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
		// Clear the buffers
		float bgcolor[] = { inClearColor.r, inClearColor.g, inClearColor.b, inClearColor.a };
		DX11Resources::sDeviceContext->ClearRenderTargetView(DX11Resources::sRenderTargetView.Get(), bgcolor);
		DX11Resources::sDeviceContext->ClearDepthStencilView(DX11Resources::sDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		DX11Resources::sDeviceContext->RSSetState(DX11Resources::sRasterizerState.Get());
		DX11Resources::sDeviceContext->OMSetDepthStencilState(DX11Resources::sDepthStencilState.Get(), 0);
		DX11Resources::sDeviceContext->OMSetRenderTargets(1, DX11Resources::sRenderTargetView.GetAddressOf(), DX11Resources::sDepthStencilView.Get());

		// Set the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (FLOAT)inWindowWidth;
		viewport.Height = (FLOAT)inWindowHeight;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		DX11Resources::sDeviceContext->RSSetViewports(1, &viewport);
	}
	
	void DX11Renderer::EndFrame(uint32_t inIndexCount)
	{
		// TODO: Implement shader first
		// Draw to screen
	//	DX11Resources::sDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	DX11Resources::sDeviceContext->DrawIndexed(inIndexCount, 0, 0);
	}
}
