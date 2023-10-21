#pragma once

#include "Core/Core.h"

#include <string>

namespace ShaderSystem
{
	enum class UniformLayoutDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		UInt, UInt2, UInt3, UInt4,
		Bool
	};

	struct UniformVariable
	{
		std::string Name;
		UniformLayoutDataType Type = UniformLayoutDataType::Float;
		uint32_t TypeCount = 1;
		uint32_t Size = 0;
		uint32_t Offset = 0;

		UniformVariable(const std::string &inName, UniformLayoutDataType inType, uint32_t inTypeCount = 1, uint32_t inOffset = 0, uint32_t inSize = 0)
			: Name(inName), Type(inType), TypeCount(inTypeCount), Size(inSize), Offset(inOffset)
		{
			if (inSize == 0)
			{
				Size = GetComponentSize();
			}
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case UniformLayoutDataType::Float:		return 1;
			case UniformLayoutDataType::Float2:		return 2;
			case UniformLayoutDataType::Float3:		return 3;
			case UniformLayoutDataType::Float4:		return 4;
			case UniformLayoutDataType::Mat3:		return 3 * 3;
			case UniformLayoutDataType::Mat4:		return 4 * 4;
			case UniformLayoutDataType::Int:		return 1;
			case UniformLayoutDataType::UInt:		return 1;
			case UniformLayoutDataType::Int2:		return 2;
			case UniformLayoutDataType::UInt2:		return 2;
			case UniformLayoutDataType::Int3:		return 3;
			case UniformLayoutDataType::UInt3:		return 3;
			case UniformLayoutDataType::Int4:		return 4;
			case UniformLayoutDataType::UInt4:		return 4;
			case UniformLayoutDataType::Bool:		return 1;
			}

			return 0;
		}

		uint32_t GetComponentSize() const
		{
			if (Size > 0)
				return Size;

			switch (Type)
			{
			case UniformLayoutDataType::Float:		return TypeCount * sizeof(float);
			case UniformLayoutDataType::Float2:		return TypeCount * sizeof(float) * 2;
			case UniformLayoutDataType::Float3:		return TypeCount * sizeof(float) * 3;
			case UniformLayoutDataType::Float4:		return TypeCount * sizeof(float) * 4;
			case UniformLayoutDataType::Mat3:		return TypeCount * sizeof(float) * 3 * 3;
			case UniformLayoutDataType::Mat4:		return TypeCount * sizeof(float) * 4 * 4;
			case UniformLayoutDataType::Int:		return TypeCount * sizeof(int32_t);
			case UniformLayoutDataType::Int2:		return TypeCount * sizeof(int32_t) * 2;
			case UniformLayoutDataType::Int3:		return TypeCount * sizeof(int32_t) * 3;
			case UniformLayoutDataType::Int4:		return TypeCount * sizeof(int32_t) * 4;
			case UniformLayoutDataType::UInt:		return TypeCount * sizeof(uint32_t);
			case UniformLayoutDataType::UInt2:		return TypeCount * sizeof(uint32_t) * 2;
			case UniformLayoutDataType::UInt3:		return TypeCount * sizeof(uint32_t) * 3;
			case UniformLayoutDataType::UInt4:		return TypeCount * sizeof(uint32_t) * 4;
			case UniformLayoutDataType::Bool:		return TypeCount * sizeof(bool) * 2; // TODO: check why *2 is necessary
			}

			return 0;
		}
	};

	class UniformLayout
	{
	public:

		UniformLayout()
		{
		}

		UniformLayout(const std::initializer_list<UniformVariable> &elements)
			: mElements(elements)
		{
			CalculateOffsetsAndStride();
		}

		std::vector<std::string> GetNames();
		uint32_t GetStride() const { return mStride; }

	private:

		void CalculateOffsetsAndStride();

		uint32_t mStride = 0;
		std::vector<UniformVariable> mElements;
	};
}
