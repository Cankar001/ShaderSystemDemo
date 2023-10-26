#pragma once

#include "Core/Core.h"
#include "Interfaces/ShaderStorageBuffer.h"

namespace ShaderSystem
{
	class VulkanStorageBuffer : public StorageBuffer
	{
	public:

		VulkanStorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());
		virtual ~VulkanStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32_t inSize) override;

	private:


	};
}
