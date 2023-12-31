#include "ShaderUniformBuffer.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Platform/DX11/DX11UniformBuffer.h"
#include "Platform/DX12/DX12UniformBuffer.h"
#include "Platform/Metal/MetalUniformBuffer.h"
#include "Platform/Vulkan/VulkanUniformBuffer.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	UniformBuffer::UniformBuffer(uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
	{
		uint32_t offset = 0;
		uint32_t size = 0;

		for (auto &element : inLayout)
		{
			mUniformVariables[element.Name] = { element.Size, element.Offset };
			offset += element.Offset + element.Size;
			size += element.Size;
		}

		mGPUDataSize = size;
		mBinding = inBinding;
		mGPUData = malloc(mGPUDataSize);
	}

	UniformBuffer::~UniformBuffer()
	{
		free(mGPUData);
		mGPUData = nullptr;
	}

	void UniformBuffer::SetData(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
		SHADER_SYSTEM_ASSERT(inOffset < inSize);

		uint32_t elementSize = mGPUDataSize;
		if (mGPUDataSize < inSize)
		{
			SHADER_SYSTEM_WARN("UniformBuffer::SetData - the copied buffer is larger than expected!");
			SHADER_SYSTEM_ASSERT(false);
		}

		memcpy_s(mGPUData, mGPUDataSize, (void *)((char *)inData + inOffset), elementSize);
		UploadToShader();
	}

	void UniformBuffer::SetVariable(const std::string &inVarName, void *inValue)
	{
		auto &entry = mUniformVariables.find(inVarName);
		if (entry != mUniformVariables.end())
		{
			uint32_t size = entry->second.first;
			uint32_t offset = entry->second.second;
			memcpy_s((void *)((char *)mGPUData + offset), size, inValue, size);
			UploadToShader();
		}
	}

	void *UniformBuffer::GetVariable(const std::string &inVarName)
	{
		auto &entry = mUniformVariables.find(inVarName);
		if (entry != mUniformVariables.end())
		{
			uint32_t size = entry->second.first;
			uint32_t offset = entry->second.second;

			void *result = malloc(size);
			memcpy_s(result, size, (void *)((char *)mGPUData + offset), size);
			return result;
		}

		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLUniformBuffer>(inSize, inBinding, inLayout);

		case RenderingAPIType::DirectX11:
			return MakeRef<DX11UniformBuffer>(inSize, inBinding, inLayout);

		case RenderingAPIType::DirectX12:
			return MakeRef<DX12UniformBuffer>(inSize, inBinding, inLayout);

		case RenderingAPIType::Metal:
			return MakeRef<MetalUniformBuffer>(inSize, inBinding, inLayout);

		case RenderingAPIType::Vulkan:
			return MakeRef<VulkanUniformBuffer>(inSize, inBinding, inLayout);
		}
	}
}
