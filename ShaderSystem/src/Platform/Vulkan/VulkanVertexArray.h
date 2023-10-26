#pragma once

#include "Core/Core.h"
#include "Interfaces/VertexArray.h"

namespace ShaderSystem
{
	class VulkanVertexArray : public VertexArray
	{
	public:

		VulkanVertexArray();
		virtual ~VulkanVertexArray();

		virtual void Bind(const BufferLayout &inLayout) const override;
		virtual void Unbind() override;

	private:


	};
}
