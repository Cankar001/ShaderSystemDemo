#pragma once

#include "Core/Core.h"

#include "Interfaces/VertexBuffer.h"

namespace ShaderSystem
{
	class DX12VertexBuffer : public VertexBuffer
	{
	public:

		DX12VertexBuffer(void *inData, uint32_t inSize);
		DX12VertexBuffer(uint32_t inSize);
		virtual ~DX12VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset = 0) override;

	private:


	};
}
