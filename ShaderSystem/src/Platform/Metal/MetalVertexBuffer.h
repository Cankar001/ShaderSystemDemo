#pragma once

#include "Core/Core.h"
#include "Interfaces/VertexBuffer.h"

namespace ShaderSystem
{
	class MetalVertexBuffer : public VertexBuffer
	{
	public:

		MetalVertexBuffer(void *inData, uint32_t inSize);
		MetalVertexBuffer(uint32_t inSize);
		virtual ~MetalVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset = 0) override;

	private:


	};
}
