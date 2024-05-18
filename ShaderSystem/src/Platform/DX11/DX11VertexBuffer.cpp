#include "DX11VertexBuffer.h"

#include "Core/Logger.h"

namespace ShaderSystem
{
	DX11VertexBuffer::DX11VertexBuffer(void *inData, uint32_t inSize)
	{
		mBufferSize = (UINT)inSize;
		mStride = std::make_unique<UINT>(sizeof(inData));

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DYNAMIC;
		vbedsc.ByteWidth = inSize;
		vbedsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbedsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = inData;

		HRESULT hr = DX11Resources::sDevice->CreateBuffer(&vbedsc, &vbdata, mBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create the vertex buffer!");
		}
	}

	DX11VertexBuffer::DX11VertexBuffer(uint32_t inSize)
	{
		mBufferSize = inSize;
		mStride = std::make_unique<UINT>(inSize);

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DYNAMIC;
		vbedsc.ByteWidth = inSize;
		vbedsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbedsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbedsc.MiscFlags = 0;

		DX11Resources::sDevice->CreateBuffer(&vbedsc, nullptr, mBuffer.GetAddressOf());
	}
	
	DX11VertexBuffer::~DX11VertexBuffer()
	{
		mBuffer.ReleaseAndGetAddressOf();
		mStride.reset();
	}
	
	void DX11VertexBuffer::Bind() const
	{
		UINT offset = 0;
		UINT stride = *mStride;
		DX11Resources::sDeviceContext->IASetVertexBuffers(0, 1, mBuffer.GetAddressOf(), &stride, &offset);
	}
	
	void DX11VertexBuffer::Unbind() const
	{
		// NOTE: not used by dx11
	}
	
	void DX11VertexBuffer::UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = DX11Resources::sDeviceContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (SUCCEEDED(hr))
		{
			memcpy(static_cast<char *>(mappedResource.pData) + inOffset, inData, inSize);
			DX11Resources::sDeviceContext->Unmap(mBuffer.Get(), 0);
		}
		else
		{
			SHADER_SYSTEM_ERROR("Failed to update the vertex data!");
		}
	}
}
