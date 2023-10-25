#include "DX11Shader.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

#include <comdef.h>
#include <dxc/dxcapi.h>

namespace ShaderSystem
{
	namespace utils
	{
		static DXGI_FORMAT BufferTypeToDX11Type(ShaderDataType inType)
		{
			switch (inType)
			{
			case ShaderDataType::Float:
				return DXGI_FORMAT_D32_FLOAT;
			case ShaderDataType::Int:
			case ShaderDataType::Bool:
				return DXGI_FORMAT_R32_SINT;
			case ShaderDataType::Float2:
				return DXGI_FORMAT_R32G32_FLOAT;
			case ShaderDataType::Float3:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case ShaderDataType::Float4:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case ShaderDataType::Int2:
				return DXGI_FORMAT_R32G32_SINT;
			case ShaderDataType::Int3:
				return DXGI_FORMAT_R32G32B32_SINT;
			case ShaderDataType::Int4:
				return DXGI_FORMAT_R32G32B32A32_SINT;

				// NOTE: We don't support matrix elements for vertex attributes. 
				// If you really need this, just declare N vectors.
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			case ShaderDataType::None:
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}

	DX11Shader::DX11Shader(const BufferLayout &inLayout)
	{
		mLayoutElements = new D3D11_INPUT_ELEMENT_DESC[inLayout.GetElementCount()];
		mLayoutElementCount = inLayout.GetElementCount();

		UINT i = 0;
		for (const auto &element : inLayout)
		{
			mLayoutElements[i].Format = utils::BufferTypeToDX11Type(element.Type);
			mLayoutElements[i].InputSlot = i;
			mLayoutElements[i].SemanticName = element.Name.c_str();
			mLayoutElements[i].SemanticIndex = i;
			++i;
		}
	}

	DX11Shader::~DX11Shader()
	{
		delete[] mLayoutElements;
	}
	
	void DX11Shader::Bind() const
	{

		if (mVertexShader)
		{
			auto it = mInputLayouts.find(ShaderDomain::Vertex);
			if (it != mInputLayouts.end())
			{
				DX11Resources::sDeviceContext->IASetInputLayout(it->second.Get());
			}

			DX11Resources::sDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		}
		else if (mPixelShader)
		{
			auto it = mInputLayouts.find(ShaderDomain::Fragment);
			if (it != mInputLayouts.end())
			{
				DX11Resources::sDeviceContext->IASetInputLayout(it->second.Get());
			}			
			DX11Resources::sDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
		}
		else if (mComputeShader)
		{
			auto it = mInputLayouts.find(ShaderDomain::Compute);
			if (it != mInputLayouts.end())
			{
				DX11Resources::sDeviceContext->IASetInputLayout(it->second.Get());
			}
			DX11Resources::sDeviceContext->CSSetShader(mComputeShader.Get(), nullptr, 0);
		}
		else if (mGeometryShader)
		{
			auto it = mInputLayouts.find(ShaderDomain::Geometry);
			if (it != mInputLayouts.end())
			{
				DX11Resources::sDeviceContext->IASetInputLayout(it->second.Get());
			}
			DX11Resources::sDeviceContext->GSSetShader(mGeometryShader.Get(), nullptr, 0);
		}
		else if (mHullShader)
		{
			auto it = mInputLayouts.find(ShaderDomain::TessControl);
			if (it != mInputLayouts.end())
			{
				DX11Resources::sDeviceContext->IASetInputLayout(it->second.Get());
			}
			DX11Resources::sDeviceContext->HSSetShader(mHullShader.Get(), nullptr, 0);
		}
		else if (mDomainShader)
		{
			auto it = mInputLayouts.find(ShaderDomain::TessEvalulation);
			if (it != mInputLayouts.end())
			{
				DX11Resources::sDeviceContext->IASetInputLayout(it->second.Get());
			}
			DX11Resources::sDeviceContext->DSSetShader(mDomainShader.Get(), nullptr, 0);
		}
	}
	
	void DX11Shader::Unbind()
	{
	}
	
	uint32_t DX11Shader::GetRendererID() const
	{
		return 0;
	}
	
	void DX11Shader::ReleaseAllDomains()
	{
		mTextDomains.clear();
		mBinaryDomains.clear();
	}
	
	bool DX11Shader::LinkAllShaders()
	{
		return true;
	}

	void DX11Shader::AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain)
	{
		HRESULT hr = S_OK;
		std::string target = "vs_5_0";

		switch (inDomain)
		{
		case ShaderDomain::Fragment:
			target = "ps_5_0";
			break;
		case ShaderDomain::Compute:
			target = "cs_5_0";
			break;
		}

		// Compile the vertex shader code
		ComPtr<ID3DBlob> blob = nullptr;
		ID3DBlob *errorBlob = nullptr;
		hr = D3DCompile(
			inSourceString.c_str(),
			inSourceString.size(),
			nullptr, // Optional file name for better debugging
			nullptr, // Macro definitions
			nullptr, // Include handler
			"main",
			target.c_str(), // Target
			0,       // Compile flags
			0,       // Effect flags
			blob.ReleaseAndGetAddressOf(),
			&errorBlob);

		if (FAILED(hr))
		{
			// Handle compilation error
			if (errorBlob)
			{
				SHADER_SYSTEM_ERROR("Error: {0}", (char *)errorBlob->GetBufferPointer());
				SHADER_SYSTEM_ASSERT(false);
				errorBlob->Release();
			}
		}

		if (SUCCEEDED(hr))
		{
			if (inDomain == ShaderDomain::Vertex)
			{
				hr = DX11Resources::sDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					SHADER_SYSTEM_ERROR("Failed to load the vertex shader binary.");
				}
			}
			else if (inDomain == ShaderDomain::Fragment)
			{
				hr = DX11Resources::sDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					SHADER_SYSTEM_ERROR("Failed to load the fragment shader binary.");
				}
			}
			else if (inDomain == ShaderDomain::Compute)
			{
				hr = DX11Resources::sDevice->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mComputeShader.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					SHADER_SYSTEM_ERROR("Failed to load the compute shader binary.");
				}
			}
			else if (inDomain == ShaderDomain::Geometry)
			{
				hr = DX11Resources::sDevice->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mGeometryShader.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					SHADER_SYSTEM_ERROR("Failed to load the geometry shader binary.");
				}
			}
			else if (inDomain == ShaderDomain::TessControl)
			{
				hr = DX11Resources::sDevice->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mHullShader.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					SHADER_SYSTEM_ERROR("Failed to load the hull/tesselation control shader binary.");
				}
			}
			else if (inDomain == ShaderDomain::TessEvalulation)
			{
				hr = DX11Resources::sDevice->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mDomainShader.ReleaseAndGetAddressOf());
				if (FAILED(hr))
				{
					SHADER_SYSTEM_ERROR("Failed to load domain/tesselation evaluation shader binary.");
				}
			}
			else
			{
				SHADER_SYSTEM_ERROR("Fatal: Failed to load shader binary, unknown shader stage.");
				SHADER_SYSTEM_ASSERT(false);
			}
		}

		hr = DX11Resources::sDevice->CreateInputLayout(mLayoutElements, mLayoutElementCount, blob->GetBufferPointer(), blob->GetBufferSize(), mInputLayouts[inDomain].ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create the input layout");
		}

		mTextDomains[inDomain] = inSourceString;
	}

	const std::string &DX11Shader::GetShaderDomainSource(ShaderDomain inDomain) const
	{
		for (const auto &[d, s] : mTextDomains)
		{
			if (d == inDomain)
			{
				return s;
			}
		}

		return std::string();
	}

	void DX11Shader::AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain)
	{
		const size_t bufferSize = inSourceBinary.size() * sizeof(uint32_t);

	//	ComPtr<IDxcUtils> pUtils;
	//	DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(pUtils.GetAddressOf()));
	//	ComPtr<IDxcBlobEncoding> blob;
	//	pUtils->CreateBlob(inSourceBinary.data(), bufferSize, CP_UTF8, blob.GetAddressOf());

		if (inDomain == ShaderDomain::Vertex)
		{
			HRESULT hr = DX11Resources::sDevice->CreateVertexShader(inSourceBinary.data(), bufferSize, nullptr, mVertexShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to load the vertex shader binary.");
			}
		}
		else if (inDomain == ShaderDomain::Fragment)
		{
			HRESULT hr = DX11Resources::sDevice->CreatePixelShader(inSourceBinary.data(), bufferSize, nullptr, mPixelShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to load the fragment shader binary.");
			}
		}
		else if (inDomain == ShaderDomain::Compute)
		{
			HRESULT hr = DX11Resources::sDevice->CreateComputeShader(inSourceBinary.data(), bufferSize, nullptr, mComputeShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to load the compute shader binary.");
			}
		}
		else if (inDomain == ShaderDomain::Geometry)
		{
			HRESULT hr = DX11Resources::sDevice->CreateGeometryShader(inSourceBinary.data(), bufferSize, nullptr, mGeometryShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to load the geometry shader binary.");
			}
		}
		else if (inDomain == ShaderDomain::TessControl)
		{
			HRESULT hr = DX11Resources::sDevice->CreateHullShader(inSourceBinary.data(), bufferSize, nullptr, mHullShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to load the hull/tesselation control shader binary.");
			}
		}
		else if (inDomain == ShaderDomain::TessEvalulation)
		{
			HRESULT hr = DX11Resources::sDevice->CreateDomainShader(inSourceBinary.data(), bufferSize, nullptr, mDomainShader.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to load domain/tesselation evaluation shader binary.");
			}
		}
		else
		{
			SHADER_SYSTEM_ERROR("Fatal: Failed to load shader binary, unknown shader stage.");
			SHADER_SYSTEM_ASSERT(false);
		}

		HRESULT hr = DX11Resources::sDevice->CreateInputLayout(mLayoutElements, mLayoutElementCount, inSourceBinary.data(), bufferSize, mInputLayouts[inDomain].ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			SHADER_SYSTEM_ERROR("Failed to create the input layout");
		}

		mBinaryDomains[inDomain] = inSourceBinary;
	}

	const std::vector<uint32_t> &DX11Shader::GetShaderDomainBinary(ShaderDomain inDomain) const
	{
		for (const auto &[d, b] : mBinaryDomains)
		{
			if (d == inDomain)
			{
				return b;
			}
		}

		return std::vector<uint32_t>();
	}
}
