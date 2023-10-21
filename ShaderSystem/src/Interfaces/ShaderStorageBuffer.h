#pragma once

#include "Core/Core.h"

#include "Graphics/ShaderUniformLayout.h"

#include <string>
#include <vector>
#include <map>

namespace ShaderSystem
{
	class StorageBuffer
	{
	public:

		virtual ~StorageBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		uint32_t GetBinding() const { return mBinding; }

		void SetData(const void *inData, uint32_t inSize, uint32_t inOffset = 0);
		void SetVariable(const std::string &inVarName, void *inValue);
		void *GetVariable(const std::string &inVarName);

		virtual void UploadToShader() = 0;
		virtual void Resize(uint32_t inSize) = 0;

		static Ref<StorageBuffer> Create(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());

	protected:

		StorageBuffer(uint32_t inBinding, const std::vector<UniformVariable> &inLayout);

	protected:

		// String --> name
		// Pair.first  --> size
		// Pair.second --> offset
		std::map<std::string, std::pair<uint32_t, uint32_t>> mUniformVariables;

		uint32_t mBinding = 0;

		void *mGPUData = nullptr;
		uint32_t mGPUDataSize = 0;
	};
}
