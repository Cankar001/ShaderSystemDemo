#include "BufferLayout.h"

namespace ShaderSystem
{
	BufferLayout BufferLayout::GetFlatColorShaderLayout()
	{
		return {
			BufferElement("a_Position", ShaderDataType::Float3, false),
			BufferElement("a_TexCoord", ShaderDataType::Float2, false),
			BufferElement("a_Color", ShaderDataType::Float4, false)
		};
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		mStride = 0;
		for (auto& element : mElements)
		{
			element.Offset = offset;
			offset += element.Size;
			mStride += element.Size;
		}
	}
}
