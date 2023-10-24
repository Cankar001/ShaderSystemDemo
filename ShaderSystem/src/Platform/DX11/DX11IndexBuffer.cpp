#include "DX11IndexBuffer.h"

namespace ShaderSystem
{
	DX11IndexBuffer::DX11IndexBuffer(const std::vector<int32_t> &inIndices)
	{
		mBufferSize = (UINT)(inIndices.size() * sizeof(int32_t));
		mCount = (uint32_t)inIndices.size();

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DEFAULT;
		vbedsc.ByteWidth = sizeof(int32_t) * (UINT)inIndices.size();
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &inIndices[0];

		DX11Resources::sDevice->CreateBuffer(&vbedsc, &vbdata, mBuffer.GetAddressOf());
	}

	DX11IndexBuffer::DX11IndexBuffer(const void *inData, uint32_t inSize)
	{
		mBufferSize = (UINT)inSize;
		mCount = (uint32_t)(inSize / sizeof(int32_t));

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DEFAULT;
		vbedsc.ByteWidth = sizeof(int32_t) * (UINT)inSize;
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &inData;

		DX11Resources::sDevice->CreateBuffer(&vbedsc, &vbdata, mBuffer.GetAddressOf());
	}
	
	DX11IndexBuffer::DX11IndexBuffer(uint32_t inSize)
	{
		mBufferSize = (UINT)inSize;
		mCount = (uint32_t)(inSize / sizeof(int32_t));

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DYNAMIC;
		vbedsc.ByteWidth = sizeof(int32_t) * (UINT)inSize;
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		DX11Resources::sDevice->CreateBuffer(&vbedsc, nullptr, mBuffer.GetAddressOf());
	}
	
	DX11IndexBuffer::~DX11IndexBuffer()
	{
	}
	
	void DX11IndexBuffer::Bind() const
	{
		DX11Resources::sDeviceContext->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	
	void DX11IndexBuffer::Unbind() const
	{
	}
	
	void DX11IndexBuffer::UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
		// TODO: implement
	}
	
	void DX11IndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
		// TODO: implement
	}
}
