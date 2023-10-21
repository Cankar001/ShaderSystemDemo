#include "ShaderStorageBufferSet.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

namespace ShaderSystem
{
	StorageBufferSet::StorageBufferSet(uint32_t inFrames)
		: mFrames(inFrames)
	{
	}

	StorageBufferSet::~StorageBufferSet()
	{
		mStorageBuffers.clear();
	}
	
	void StorageBufferSet::CreateStorage(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
	{
		for (uint32_t frame = 0; frame < mFrames; ++frame)
		{
			const Ref<StorageBuffer> storageBuffer = StorageBuffer::Create(inSize, inBinding, inLayout);
			SetStorage(storageBuffer, 0, frame);
		}
	}
	
	Ref<StorageBuffer> StorageBufferSet::GetStorage(uint32_t inBinding, uint32_t inSet, uint32_t inFrame)
	{
		SHADER_SYSTEM_ASSERT(mStorageBuffers.find(inFrame) != mStorageBuffers.end());
		SHADER_SYSTEM_ASSERT(mStorageBuffers.at(inFrame).find(inSet) != mStorageBuffers.at(inFrame).end());
		SHADER_SYSTEM_ASSERT(mStorageBuffers.at(inFrame).at(inSet).find(inBinding) != mStorageBuffers.at(inFrame).at(inSet).end());

		return mStorageBuffers.at(inFrame).at(inSet).at(inBinding);
	}
	
	void StorageBufferSet::SetStorage(const Ref<StorageBuffer> &inStorageBuffer, uint32_t inSet, uint32_t inFrame)
	{
		mStorageBuffers[inFrame][inSet][inStorageBuffer->GetBinding()] = inStorageBuffer;
	}
	
	void StorageBufferSet::Resize(uint32_t inBinding, uint32_t inSet, uint32_t inSize)
	{
		// NOTE: Nothing to do, if there are no storage buffers stored
		if (!mStorageBuffers.size())
			return;

		for (uint32_t frame = 0; frame < mFrames; ++frame)
		{
			mStorageBuffers.at(frame).at(inSet).at(inBinding)->Resize(inSize);
		}
	}
	
	void StorageBufferSet::ForEach(const StorageBufferCallback &inCallback) const
	{
		for (uint32_t frame = 0; frame < mFrames; ++frame)
		{
			for (auto &[set, buffers] : mStorageBuffers.at(frame))
			{
				for (auto &[binding, sb] : buffers)
				{
					SHADER_SYSTEM_ASSERT(sb);
					inCallback(sb);
				}
			}
		}
	}
	
	Ref<StorageBufferSet> StorageBufferSet::Create(uint32_t inFrames)
	{
		return MakeRef<StorageBufferSet>(inFrames);
	}
}
