#pragma once

#include "Interfaces/GPUShader.h"

#include "Graphics/BufferLayout.h"

namespace ShaderSystem
{
	class OpenGLShader : public GPUShader
	{
	public:

		OpenGLShader(const BufferLayout &inLayout);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() override;
		virtual uint32_t GetRendererID() const override { return mProgramID; }

		virtual void ReleaseAllDomains() override;
		virtual bool LinkAllShaders() override;

		virtual void AddShaderDomain(const std::string& inSourceString, ShaderDomain inDomain) override;
		virtual std::string GetShaderDomainSource(ShaderDomain inDomain) const override;

		virtual void AddShaderDomain(const std::vector<uint32_t>& inSourceBinary, ShaderDomain inDomain) override;
		virtual std::vector<uint32_t> GetShaderDomainBinary(ShaderDomain inDomain) const override;

	private:

		uint32_t mProgramID;
		std::unordered_map<ShaderDomain, uint32_t> mShaderHandles;

		//	std::vector<OpenGLShaderDescriptorSet> mShaderDescriptorSets;
		//	std::vector<OpenGLShaderPushConstantRange> mPushConstantRanges;
	};
}
