#pragma once

#include "Core/Core.h"

#include "Interfaces/ShaderStorageBuffer.h"

namespace ShaderSystem
{
	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:

		OpenGLStorageBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());
		virtual ~OpenGLStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32_t inSize) override;

	private:

		uint32_t mRendererID;
	};
}
