#pragma once

#include "Core/Core.h"
#include "Interfaces/ShaderUniformBuffer.h"

namespace ShaderSystem
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());
		virtual ~VulkanUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UploadToShader() override;

	private:


	};
}
