#pragma once

#include "Core/Core.h"
#include "Graphics/VertexBuffer.h"

namespace ShaderSystem
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:

		OpenGLVertexBuffer(void *inData, uint32_t inSize);
		OpenGLVertexBuffer(uint32_t inSize);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset = 0) override;

	private:

		uint32_t mRendererID = 0;
		void *mData = nullptr;
		uint32_t mSize = 0;
	};
}
