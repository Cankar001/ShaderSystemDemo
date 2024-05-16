#pragma once

#include "Core/Core.h"
#include "Interfaces/ShaderUniformBuffer.h"

#include "DX11Resources.h"

namespace ShaderSystem
{
	class DX11UniformBuffer : public UniformBuffer
	{
	public:

		DX11UniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout);
		virtual ~DX11UniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;

	private:

		ComPtr<ID3D11Buffer> mBuffer;
	};
}
