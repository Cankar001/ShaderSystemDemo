#pragma once

#include "Core/Core.h"

namespace ShaderSystem
{
	class VertexBuffer
	{
	public:

		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset = 0) = 0;

		static Ref<VertexBuffer> Create(void *inData, uint32_t inSize);
		static Ref<VertexBuffer> Create(uint32_t inSize);
	};
}
