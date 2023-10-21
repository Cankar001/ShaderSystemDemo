#pragma once

#include "Core/Core.h"
#include "BufferLayout.h"

namespace ShaderSystem
{
	class VertexArray
	{
	public:

		virtual ~VertexArray() {}

		virtual void Bind(const BufferLayout &inLayout) const = 0;
		virtual void Unbind() = 0;

		static Ref<VertexArray> Create();
	};
}
