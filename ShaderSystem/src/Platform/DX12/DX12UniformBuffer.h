#pragma once

#include "Core/Core.h"

#include "Interfaces/ShaderUniformBuffer.h"

namespace ShaderSystem
{
	class DX12UniformBuffer : public UniformBuffer
	{
	public:

		DX12UniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout);
		virtual ~DX12UniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;

	private:


	};
}
