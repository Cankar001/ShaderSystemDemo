#include "ShaderUniformLayout.h"

namespace ShaderSystem
{
	std::vector<std::string> UniformLayout::GetNames()
	{
		std::vector<std::string> result;

		for (auto &element : mElements)
		{
			result.push_back(element.Name);
		}

		return result;
	}

	void UniformLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		mStride = 0;
		for (auto &element : mElements)
		{
			element.Offset = offset;
			offset += element.Size;
			mStride += element.Size;
		}
	}
}
