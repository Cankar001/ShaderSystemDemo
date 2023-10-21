#include "ShaderUniformBufferSet.h"

#include "Core/Assert.h"
#include "Core/Logger.h"

namespace ShaderSystem
{
	UniformBufferSet::UniformBufferSet(uint32_t inFrames)
		: mFrames(inFrames)
	{
	}

	UniformBufferSet::~UniformBufferSet()
	{
		mUniformBuffers.clear();
	}
	
	void UniformBufferSet::CreateUniform(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout)
	{
		for (uint32_t frame = 0; frame < mFrames; ++frame)
		{
			Ref<UniformBuffer> uniformBuffer = UniformBuffer::Create(inSize, inBinding, inLayout);
			SetUniform(uniformBuffer, 0, frame);
		}
	}
	
	Ref<UniformBuffer> UniformBufferSet::GetUniform(uint32_t inBinding, uint32_t inSet, uint32_t inFrame)
	{
		SHADER_SYSTEM_ASSERT(mUniformBuffers.find(inFrame) != mUniformBuffers.end());
		SHADER_SYSTEM_ASSERT(mUniformBuffers.at(inFrame).find(inSet) != mUniformBuffers.at(inFrame).end());
		SHADER_SYSTEM_ASSERT(mUniformBuffers.at(inFrame).at(inSet).find(inBinding) != mUniformBuffers.at(inFrame).at(inSet).end());

		return mUniformBuffers.at(inFrame).at(inSet).at(inBinding);
	}
	
	void UniformBufferSet::SetUniform(const Ref<UniformBuffer> &inUniform, uint32_t inSet, uint32_t inFrame)
	{
		mUniformBuffers[inFrame][inSet][inUniform->GetBinding()] = inUniform;
	}
	
	void UniformBufferSet::ForEach(const UniformBufferCallback &inCallback) const
	{
		for (uint32_t frame = 0; frame < mFrames; ++frame)
		{
			for (auto &[set, buffer] : mUniformBuffers.at(frame))
			{
				for (auto &[binding, ub] : buffer)
				{
					SHADER_SYSTEM_ASSERT(ub);
					inCallback(ub);
				}
			}
		}
	}
	
	Ref<UniformBufferSet> UniformBufferSet::Create(uint32_t inFrames)
	{
		return MakeRef<UniformBufferSet>(inFrames);
	}
}
