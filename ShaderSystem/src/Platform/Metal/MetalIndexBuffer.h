#pragma once

#include "Core/Core.h"
#include "Interfaces/IndexBuffer.h"

namespace ShaderSystem
{
	class MetalIndexBuffer : public IndexBuffer
	{
	public:

		MetalIndexBuffer(const std::vector<int32_t> &inIndices);
		MetalIndexBuffer(const void *inData, uint32_t inSize);
		MetalIndexBuffer(uint32_t inSize);
		virtual ~MetalIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() override { return mCount; }
		virtual void UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset = 0) override;
		virtual void UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset = 0) override;

	private:

		uint32_t mCount = 0;
	};
}
