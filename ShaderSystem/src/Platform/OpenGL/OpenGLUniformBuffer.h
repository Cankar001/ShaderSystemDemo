#pragma once

#include "Core/Core.h"

#include "Interfaces/ShaderUniformBuffer.h"

namespace ShaderSystem
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:

		OpenGLUniformBuffer(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout);
		virtual ~OpenGLUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UploadToShader() override;

	private:

		uint32_t mRendererID = 0;
	};
}
