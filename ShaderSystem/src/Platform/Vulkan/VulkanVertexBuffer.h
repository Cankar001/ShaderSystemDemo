#pragma once

#include "Core/Core.h"
#include "Interfaces/VertexBuffer.h"

namespace ShaderSystem
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:

		VulkanVertexBuffer(void *inData, uint32_t inSize);
		VulkanVertexBuffer(uint32_t inSize);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *inData, uint32_t inSize, uint32_t inOffset = 0) override;

	private:


	};
}
