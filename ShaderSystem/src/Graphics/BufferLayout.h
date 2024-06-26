#pragma once

#include "Core/Core.h"

#include <string>

namespace ShaderSystem
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	namespace utils
	{
		static uint32_t ShaderDataTypeSize(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
			}

			return 0;
		}
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type = ShaderDataType::Float;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		bool Normalized = false;

		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), Size(utils::ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}
	};

	class BufferLayout
	{
	public:

		static BufferLayout Empty;

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: mElements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return mStride; }
		inline const std::vector<BufferElement>& GetElements() const { return mElements; }
		inline uint32_t GetElementCount() const { return (uint32_t)mElements.size(); }

		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

		static BufferLayout GetFlatColorShaderLayout();

	private:

		void CalculateOffsetsAndStride();

		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;
	};
}
