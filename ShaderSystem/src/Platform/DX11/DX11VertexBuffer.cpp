#include "DX11VertexBuffer.h"

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
		vbdata.pSysMem = &inData;

		DX11Resources::sDevice->CreateBuffer(&vbedsc, &vbdata, mBuffer.GetAddressOf());
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
		// NOTE: not used by dx11
	}
	
	void DX11VertexBuffer::Unbind() const
	{
		// NOTE: not used by dx11
	}
	
	void DX11VertexBuffer::UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset)
	{
		// TODO: implement
	}
}
