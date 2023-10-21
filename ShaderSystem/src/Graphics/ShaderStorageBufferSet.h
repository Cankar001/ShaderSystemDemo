#pragma once

#include "Core/Core.h"
#include "Interfaces/ShaderStorageBuffer.h"

#include <map>

namespace ShaderSystem
{
	class StorageBufferSet
	{
	public:

		using StorageBufferCallback = std::function<void(const Ref<StorageBuffer> &)>;

		StorageBufferSet(uint32_t inFrames);
		virtual ~StorageBufferSet();

		void CreateStorage(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout);

		Ref<StorageBuffer> GetStorage(uint32_t inBinding, uint32_t inSet = 0, uint32_t inFrame = 0);
		void SetStorage(const Ref<StorageBuffer> &inStorageBuffer, uint32_t inSet = 0, uint32_t inFrame = 0);
		void Resize(uint32_t inBinding, uint32_t inSet, uint32_t inSize);

		void ForEach(const StorageBufferCallback &inCallback) const;

		static Ref<StorageBufferSet> Create(uint32_t inFrames);

	private:

		uint32_t mFrames;
		std::map<uint32_t, std::map<uint32_t, std::map<uint32_t, Ref<StorageBuffer>>>> mStorageBuffers; // frame -> set -> binding
	};
}
