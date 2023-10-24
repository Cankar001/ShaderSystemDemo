#pragma once

#include "Core/Core.h"
#include "Interfaces/VertexBuffer.h"

#include "DX11Resources.h"

namespace ShaderSystem
{
	class DX11VertexBuffer : public VertexBuffer
	{
	public:

		DX11VertexBuffer(void *inData, uint32_t inSize);
		DX11VertexBuffer(uint32_t inSize);
		virtual ~DX11VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset = 0) override;

	private:

		ComPtr<ID3D11Buffer>	mBuffer;
		UINT					mBufferSize;
		std::unique_ptr<UINT>	mStride;
	};
}
