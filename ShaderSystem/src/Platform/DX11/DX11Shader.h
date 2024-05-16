#pragma once

#include "Core/Core.h"
#include "Interfaces/GPUShader.h"

#include "Graphics/BufferLayout.h"

#include "DX11Resources.h"

namespace ShaderSystem
{
	class DX11Shader : public GPUShader
	{
	public:

		DX11Shader(const BufferLayout &inLayout);
		virtual ~DX11Shader();

		virtual void Bind(ShaderDomain inDomain) const override;
		virtual void Unbind() override;
		virtual uint32_t GetRendererID() const override;

		virtual void ReleaseAllDomains() override;
		virtual bool LinkAllShaders() override;

		virtual void AddShaderDomain(const std::string &inSourceString, ShaderDomain inDomain) override;
		virtual const std::string &GetShaderDomainSource(ShaderDomain inDomain) const override;

		virtual void AddShaderDomain(const std::vector<uint32_t> &inSourceBinary, ShaderDomain inDomain) override;
		virtual const std::vector<uint32_t> &GetShaderDomainBinary(ShaderDomain inDomain) const override;

	private:

		ComPtr<ID3D11VertexShader> mVertexShader	 = nullptr;
		ComPtr<ID3D11PixelShader> mPixelShader		 = nullptr;
		ComPtr<ID3D11ComputeShader> mComputeShader	 = nullptr;
		ComPtr<ID3D11GeometryShader> mGeometryShader = nullptr;
		ComPtr<ID3D11DomainShader> mDomainShader	 = nullptr;
		ComPtr<ID3D11HullShader> mHullShader		 = nullptr;

		std::unordered_map<ShaderDomain, ComPtr<ID3D11InputLayout>> mInputLayouts;
		
		D3D11_INPUT_ELEMENT_DESC *mLayoutElements	 = nullptr;
		UINT mLayoutElementCount					 = 0;
	};
}
