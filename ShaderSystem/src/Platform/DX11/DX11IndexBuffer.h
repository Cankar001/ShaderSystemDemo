#pragma once

#include "Core/Core.h"
#include "Interfaces/IndexBuffer.h"

namespace ShaderSystem
{
	class DX11IndexBuffer : public IndexBuffer
	{
	public:

		DX11IndexBuffer(const std::vector<int32_t> &inIndices);
		DX11IndexBuffer(const void *inData, uint32_t inSize);
		DX11IndexBuffer(uint32_t inSize);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() override { return mCount; }
		virtual void UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset = 0) override;
		virtual void UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset = 0) override;

	private:

		void *mData = nullptr;
		uint32_t mSize = 0;
		uint32_t mCount = 0;
	};
}
