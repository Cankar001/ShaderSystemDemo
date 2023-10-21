#pragma once

#include "Core/Core.h"
#include "Graphics/ShaderUniformLayout.h"

#include <map>
#include <vector>
#include <string>

namespace ShaderSystem
{
	class UniformBuffer
	{
	public:

		virtual ~UniformBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		void SetData(const void *inData, uint32_t inSize, uint32_t inOffset = 0);
		void SetVariable(const std::string &inVarName, void *inValue);
		void *GetVariable(const std::string &inVarName);

		virtual void UploadToShader() = 0;
		uint32_t GetBinding() const { return mBinding; }

		static Ref<UniformBuffer> Create(uint32_t inSize, uint32_t inBinding, const std::vector<UniformVariable> &inLayout = std::vector<UniformVariable>());

	protected:

		UniformBuffer(uint32_t inBinding, const std::vector<UniformVariable> &inLayout);

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
