#pragma once

#include "Core/Core.h"
#include "Interfaces/ShaderStorageBuffer.h"

namespace ShaderSystem
{
	class MetalStorageBuffer : public StorageBuffer
	{
	public:

		MetalStorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());
		virtual ~MetalStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32_t inSize) override;

	private:


	};
}
