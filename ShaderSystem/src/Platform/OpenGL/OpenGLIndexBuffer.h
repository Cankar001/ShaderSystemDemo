#pragma once

#include "Core/Core.h"

#include "Graphics/IndexBuffer.h"

namespace ShaderSystem
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:

		OpenGLIndexBuffer(const std::vector<int32_t> &inIndices);
		OpenGLIndexBuffer(const void *inData, uint32_t inSize);
		OpenGLIndexBuffer(uint32_t inSize);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() override { return mCount; }
		virtual void UpdateContents(const void *inData, uint32_t inSize, uint32_t inOffset = 0) override;
		virtual void UpdateContents(const std::vector<int32_t> &inIndices, uint32_t inOffset = 0) override;

	private:

		uint32_t mRendererID = 0;
		void *mData = nullptr;
		uint32_t mSize = 0;
		uint32_t mCount = 0;
	};
}
