#pragma once

#include "Core/Core.h"
#include "Interfaces/VertexArray.h"

namespace ShaderSystem
{
	class DX11VertexArray : public VertexArray
	{
	public:

		DX11VertexArray();
		virtual ~DX11VertexArray();

		virtual void Bind(const BufferLayout &inLayout) const override;
		virtual void Unbind() override;
	};
}
