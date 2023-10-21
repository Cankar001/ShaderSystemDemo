#pragma once

#include "Core/Core.h"

namespace ShaderSystem
{
	class IndexBuffer
	{
	public:

		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() = 0;
		virtual void UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset = 0) = 0;
		virtual void UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset = 0) = 0;

		static Ref<IndexBuffer> Create(const std::vector<int32_t> &inIndices);
		static Ref<IndexBuffer> Create(const void *inData, uint32_t inSize);
		static Ref<IndexBuffer> Create(uint32_t inSize);
	};
}
