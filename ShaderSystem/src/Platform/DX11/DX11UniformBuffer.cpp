#include "DX11UniformBuffer.h"

#include "Core/Logger.h"

namespace ShaderSystem
{
	DX11UniformBuffer::DX11UniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
		: UniformBuffer(inBinding, inLayout)
	{
		D3D11_BUFFER_DESC uniformBuffer = { 0 };
		uniformBuffer.Usage = D3D11_USAGE_DYNAMIC;
		uniformBuffer.ByteWidth = inSize;
		uniformBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		uniformBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = DX11Resources::sDevice->CreateBuffer(&uniformBuffer, NULL, mBuffer.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create uniform buffer!");
		}
	}

	DX11UniformBuffer::~DX11UniformBuffer()
	{
		//mBuffer->Release();
	}
	
	void DX11UniformBuffer::Bind() const
	{
		DX11Resources::sDeviceContext->VSSetConstantBuffers(mBinding, 1, mBuffer.GetAddressOf());
	}
	
	void DX11UniformBuffer::Unbind() const
	{
		ID3D11Buffer *nullBuffer = nullptr;
		DX11Resources::sDeviceContext->VSSetConstantBuffers(mBinding, 1, &nullBuffer);
	}
	
	void DX11UniformBuffer::UploadToShader()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = DX11Resources::sDeviceContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (SUCCEEDED(result))
		{
			memcpy(mappedResource.pData, mGPUData, mGPUDataSize); 
			DX11Resources::sDeviceContext->Unmap(mBuffer.Get(), 0);
		}
		else
		{
			SHADER_SYSTEM_ERROR("Failed to upload the data onto the gpu!");
		}
	}
}
