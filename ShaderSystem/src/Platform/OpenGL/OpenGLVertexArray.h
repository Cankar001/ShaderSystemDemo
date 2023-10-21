#pragma once

#include "Core/Core.h"
#include "Graphics/VertexArray.h"

namespace ShaderSystem
{
	class OpenGLVertexArray : public VertexArray
	{
	public:

		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind(const BufferLayout &inLayout) const override;
		virtual void Unbind() override;

	private:

		uint32_t mRendererID;
	};
}
