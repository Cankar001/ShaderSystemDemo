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

		static UINT SizeOfFormatElement(ShaderDataType inType)
		{
			switch (inType)
			{
			case ShaderDataType::Float:
				return 4;
			case ShaderDataType::Int:
				return 4;
			case ShaderDataType::Bool:
				return 1;
			case ShaderDataType::Float2:
				return 8;
			case ShaderDataType::Float3:
				return 12;
			case ShaderDataType::Float4:
				return 16;
			case ShaderDataType::Int2:
				return 8;
			case ShaderDataType::Int3:
				return 12;
			case ShaderDataType::Int4:
				return 16;

			case ShaderDataType::Mat3:
				return 3 * 3 * 4;

			case ShaderDataType::Mat4:
				return 4 * 4 * 4;

			case ShaderDataType::None:
			default:
				return 0;
			}
		}
	}

	DX11Shader::DX11Shader(const BufferLayout &inLayout)
	{
		mLayoutElements = new D3D11_INPUT_ELEMENT_DESC[inLayout.GetElementCount()];
		mLayoutElementCount = inLayout.GetElementCount();

		UINT i = 0;
		UINT offset = 0;
		for (const auto &element : inLayout)
		{
		//	mLayoutElements[i].SemanticName = element.Name.c_str();
			mLayoutElements[i].SemanticName = "TEXCOORD"; // HACK: every name is called TEXCOORD in the SPIR-V parser
			mLayoutElements[i].SemanticIndex = i;
			mLayoutElements[i].Format = utils::BufferTypeToDX11Type(element.Type);
			mLayoutElements[i].InputSlot = 0;
			mLayoutElements[i].AlignedByteOffset = offset;
			mLayoutElements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			mLayoutElements[i].InstanceDataStepRate = 0;
			
			++i;
			offset += utils::SizeOfFormatElement(element.Type);
		}
	}

	DX11Shader::~DX11Shader()
	{
		delete[] mLayoutElements;
	}
	
	void DX11Shader::Bind() const
	{
		for (auto &inUseDomain : mShadersInUse)
		{
			if (inUseDomain == ShaderDomain::Vertex)
			{
				DX11Resources::sDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
			}
			else if (inUseDomain == ShaderDomain::Fragment)
			{
				DX11Resources::sDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
			}
			else if (inUseDomain == ShaderDomain::Compute)
			{
				DX11Resources::sDeviceContext->CSSetShader(mComputeShader.Get(), nullptr, 0);
			}
			else if (inUseDomain == ShaderDomain::Geometry)
			{
				DX11Resources::sDeviceContext->GSSetShader(mGeometryShader.Get(), nullptr, 0);
			}
			else if (inUseDomain == ShaderDomain::TessControl)
			{
				DX11Resources::sDeviceContext->HSSetShader(mHullShader.Get(), nullptr, 0);
			}
			else if (inUseDomain == ShaderDomain::TessEvalulation)
			{
				DX11Resources::sDeviceContext->DSSetShader(mDomainShader.Get(), nullptr, 0);
			}
		}

		if (mInputLayout)
			DX11Resources::sDeviceContext->IASetInputLayout(mInputLayout.Get());
	}
	
	void DX11Shader::Unbind()
	{
	}
	
	uint32_t DX11Shader::GetRendererID() const
	{
		return mRendererID;
	}
	
	void DX11Shader::ReleaseAllDomains()
	{
		mTextDomains.clear();
		mBinaryDomains.clear();
	}
	
	bool DX11Shader::LinkAllShaders()
	{
		// NOTE: Nothing to do for DX11.
		return true;
	}

	void DX11Shader::AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain)
	{
		HRESULT hr = S_OK;
		std::string version = "5_0"; // The shader version
		std::string target = "vs_" + version;

		switch (inDomain)
		{
		case ShaderDomain::Fragment:
			target = "ps_" + version;
			break;
		case ShaderDomain::Compute:
			target = "cs_" + version;
			break;
		case ShaderDomain::Geometry:
			target = "gs_" + version;
			break;
		case ShaderDomain::TessControl:
			target = "hs_" + version;
			break;
		case ShaderDomain::TessEvalulation:
			target = "ds_" + version;
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

		if (inDomain == ShaderDomain::Vertex)
		{
			SHADER_SYSTEM_INFO("Creating vertex input layout for vertex shader...");
			hr = DX11Resources::sDevice->CreateInputLayout(mLayoutElements, mLayoutElementCount, blob->GetBufferPointer(), blob->GetBufferSize(), mInputLayout.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to create the input layout!");
			}
		}

		mTextDomains[inDomain] = inSourceString;
		mShadersInUse.push_back(inDomain);
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

		if (inDomain == ShaderDomain::Vertex)
		{
			HRESULT hr = DX11Resources::sDevice->CreateInputLayout(mLayoutElements, mLayoutElementCount, inSourceBinary.data(), bufferSize, mInputLayout.ReleaseAndGetAddressOf());
			if (FAILED(hr))
			{
				SHADER_SYSTEM_ERROR("Failed to create the input layout");
			}
		}

		mBinaryDomains[inDomain] = inSourceBinary;
		mShadersInUse.push_back(inDomain);
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
