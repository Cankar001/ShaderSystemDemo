#pragma once

#include "Core/Core.h"

#include "Interfaces/VertexArray.h"

namespace ShaderSystem
{
	class DX12VertexArray : public VertexArray
	{
	public:

		DX12VertexArray();
		virtual ~DX12VertexArray();

		virtual void Bind(const BufferLayout &inLayout) const override;
		virtual void Unbind() override;
	};
}