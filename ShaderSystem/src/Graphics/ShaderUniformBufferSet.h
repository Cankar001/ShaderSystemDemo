#pragma once

#include "Core/Core.h"

#include "Interfaces/ShaderUniformBuffer.h"

#include <map>

namespace ShaderSystem
{
	class UniformBufferSet
	{
	public:

		using UniformBufferCallback = std::function<void(const Ref<UniformBuffer> &)>;

		UniformBufferSet(uint32_t inFrames);
		virtual ~UniformBufferSet();

		void CreateUniform(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout);

		Ref<UniformBuffer> GetUniform(uint32_t inBinding, uint32_t inSet = 0, uint32_t inFrame = 0);
		void SetUniform(const Ref<UniformBuffer> &inUniform, uint32_t inSet = 0, uint32_t inFrame = 0);

		void ForEach(const UniformBufferCallback &inCallback) const;

		static Ref<UniformBufferSet> Create(uint32_t inFrames);

	private:

		uint32_t mFrames;
		std::map<uint32_t, std::map<uint32_t, std::map<uint32_t, Ref<UniformBuffer>>>> mUniformBuffers; // frame -> set -> binding
	};
}
