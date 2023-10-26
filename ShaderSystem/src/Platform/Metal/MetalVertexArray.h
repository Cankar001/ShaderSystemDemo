#pragma once

#include "Core/Core.h"
#include "Interfaces/VertexArray.h"

namespace ShaderSystem
{
	class MetalVertexArray : public VertexArray
	{
	public:

		MetalVertexArray();
		virtual ~MetalVertexArray();

		virtual void Bind(const BufferLayout &inLayout) const override;
		virtual void Unbind() override;

	private:


	};
}
