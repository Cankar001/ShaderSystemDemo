#pragma once

#include "Core/Core.h"
#include "Interfaces/ShaderUniformBuffer.h"

namespace ShaderSystem
{
	class MetalUniformBuffer : public UniformBuffer
	{
	public:

		MetalUniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());
		virtual ~MetalUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UploadToShader() override;

	private:


	};
}
