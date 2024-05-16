#include "DX11IndexBuffer.h"

#include "Core/Logger.h"

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
		vbdata.pSysMem = inIndices.data();

		HRESULT hr = DX11Resources::sDevice->CreateBuffer(&vbedsc, &vbdata, mBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create index buffer!");
		}
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
		vbdata.pSysMem = inData;

		HRESULT hr = DX11Resources::sDevice->CreateBuffer(&vbedsc, &vbdata, mBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create index buffer!");
		}
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

		HRESULT hr = DX11Resources::sDevice->CreateBuffer(&vbedsc, nullptr, mBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create index buffer!");
		}
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
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = DX11Resources::sDeviceContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (SUCCEEDED(hr))
		{
			memcpy(static_cast<char *>(mappedResource.pData) + inOffset, inData, inSize);
			DX11Resources::sDeviceContext->Unmap(mBuffer.Get(), 0);
		}
		else
		{
			SHADER_SYSTEM_ERROR("Failed to update index data!");
		}
	}
	
	void DX11IndexBuffer::UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset)
	{
		UpdateContents(inIndices.data(), static_cast<uint32_t>(inIndices.size() * sizeof(int32_t)), inOffset);
	}
}
