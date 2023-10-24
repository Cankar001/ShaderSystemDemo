#include "ShaderStorageBuffer.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

#include "Platform/OpenGL/OpenGLStorageBuffer.h"
#include "Renderer/Renderer.h"

namespace ShaderSystem
{
	StorageBuffer::StorageBuffer(uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
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

	StorageBuffer::~StorageBuffer()
	{
		free(mGPUData);
		mGPUData = nullptr;
	}

	void StorageBuffer::SetData(const void *inData, uint32_t inSize, uint32_t inOffset)
	{
		SHADER_SYSTEM_ASSERT(inOffset < inSize);

		uint32_t elementSize = mGPUDataSize;
		if (mGPUDataSize < inSize)
		{
			SHADER_SYSTEM_WARN("StorageBuffer::SetData - the copied buffer is larger than expected!");
			SHADER_SYSTEM_ASSERT(false);
		}

		memcpy_s(mGPUData, mGPUDataSize, (void *)((char *)inData + inOffset), elementSize);
		UploadToShader();
	}

	void StorageBuffer::SetVariable(const std::string &inVarName, void *inValue)
	{
		auto &entry = mUniformVariables.find(inVarName);
		if (entry != mUniformVariables.end())
		{
			uint32_t size = entry->second.first;
			uint32_t offset = entry->second.second;
			memcpy_s((void *)((char *)mGPUData + offset), size, inValue, size);
		}

		UploadToShader();
	}
	
	void *StorageBuffer::GetVariable(const std::string &inVarName)
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
	
	Ref<StorageBuffer> StorageBuffer::Create(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
	{
		switch (Renderer::GetCurrentRenderingAPIType())
		{
		default:
		case RenderingAPIType::OpenGL:
			return MakeRef<OpenGLStorageBuffer>(inSize, inBinding, inLayout);

		case RenderingAPIType::DirectX11:
		case RenderingAPIType::DirectX12:
		case RenderingAPIType::Metal:
		case RenderingAPIType::Vulkan:
			return nullptr;
		}
	}
}
