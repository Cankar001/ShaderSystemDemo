#pragma once

#include "Core/Core.h"

#include "Interfaces/IndexBuffer.h"

namespace ShaderSystem
{
	class DX12IndexBuffer : public IndexBuffer
	{
	public:

		DX12IndexBuffer(const std::vector<int32_t> &inIndices);
		DX12IndexBuffer(const void *inData, uint32_t inSize);
		DX12IndexBuffer(uint32_t inSize);
		virtual ~DX12IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() override { return mCount; }
		virtual void UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset = 0) override;
		virtual void UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset = 0) override;


	private:

		uint32_t mCount = 0;
	};
}
