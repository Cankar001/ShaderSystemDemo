#include "BufferLayout.h"

namespace ShaderSystem
{
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
