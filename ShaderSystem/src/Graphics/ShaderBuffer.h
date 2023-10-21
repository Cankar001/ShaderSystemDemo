#pragma once

#include "Core/Core.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace ShaderSystem
{
	enum class ShaderUniformType
	{
		None = 0,
		Bool,
		Int, Uint,
		Float,
		Vec2, Vec3, Vec4,
		IVec2, IVec3, IVec4,
		Mat2, Mat3, Mat4,
		Struct
	};

	enum class ShaderDomain
	{
		None = 0,
		Vertex = 1,
		Fragment = 2,
		Compute = 3,
		Geometry = 4,
		TessControl = 5,
		TessEvalulation = 6
	};

	enum class ShaderLanguage
	{
		None,
		GLSL,
		HLSL
	};

	class ShaderUniform
	{
	public:

		ShaderUniform() = default;
		ShaderUniform(const std::string& inName, uint32_t inBinding, ShaderUniformType inType, uint32_t inSize, uint32_t inOffset);

		const std::string& GetName() const { return mName; }
		ShaderUniformType GetType() const { return mType; }
		uint32_t GetSize() const { return mSize; }
		uint32_t GetOffset() const { return mOffset; }
		uint32_t GetBinding() const { return mBinding; }

	private:

		std::string mName;
		ShaderUniformType mType = ShaderUniformType::None;
		uint32_t mSize = 0;
		uint32_t mOffset = 0;
		uint32_t mBinding = 0;
	};

	struct ShaderImageSampler
	{
		uint32_t BindingPoint = 0;
		uint32_t DescriptorSet = 0;
		uint32_t ArraySize = 0;
		std::string Name;
	};

	struct ShaderPushConstantRange
	{
		uint32_t Offset = 0;
		uint32_t Size = 0;
	};

	struct ShaderUniformBuffer
	{
		std::string Name;
		uint32_t Index;
		uint32_t BindingPoint;
		uint32_t Size;
		uint32_t RendererID;
		std::vector<ShaderUniform> Uniforms;
	};

	struct ShaderStorageBuffer
	{
		std::string Name;
		uint32_t Index;
		uint32_t BindingPoint;
		uint32_t Size;
		uint32_t RendererID;
	};

	struct ShaderBuffer
	{
		std::string Name;
		uint32_t Size = 0;
		std::unordered_map<std::string, ShaderUniform> Uniforms;
	};

	struct ShaderDescriptorSet
	{
		std::unordered_map<uint32_t, ShaderUniformBuffer*> UniformBuffers;
		std::unordered_map<uint32_t, ShaderStorageBuffer*> StorageBuffers;
		std::unordered_map<uint32_t, ShaderImageSampler> ImageSamplers;
		std::unordered_map<uint32_t, ShaderImageSampler> StorageImages;
		std::unordered_map<uint32_t, ShaderImageSampler> SeparateTextures;
		std::unordered_map<uint32_t, ShaderImageSampler> SeparateSamplers;

		operator bool() const
		{
			return !(StorageBuffers.empty()
				&& UniformBuffers.empty()
				&& ImageSamplers.empty()
				&& StorageImages.empty());
		}
	};

	class ShaderResourceDeclaration
	{
	public:

		ShaderResourceDeclaration() = default;
		ShaderResourceDeclaration(const std::string& inName, uint32_t inResourceRegister, uint32_t inCount);

		virtual const std::string& GetName() const { return mName; }
		virtual uint32_t GetRegister() const { return mRegister; }
		virtual uint32_t GetCount() const { return mCount; }

	private:

		std::string mName;
		uint32_t mRegister = 0;
		uint32_t mCount = 0;
	};

	typedef std::vector<ShaderResourceDeclaration*> ShaderResourceList;
}
