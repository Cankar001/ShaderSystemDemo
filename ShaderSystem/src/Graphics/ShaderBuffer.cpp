#include "ShaderBuffer.h"

namespace ShaderSystem
{
	ShaderUniform::ShaderUniform(const std::string& inName, uint32_t inBinding, ShaderUniformType inType, uint32_t inSize, uint32_t inOffset)
		: mName(inName), mBinding(inBinding), mType(inType), mSize(inSize), mOffset(inOffset)
	{
	}

	ShaderResourceDeclaration::ShaderResourceDeclaration(const std::string& inName, uint32_t inResourceRegister, uint32_t inCount)
		: mName(inName), mRegister(inResourceRegister), mCount(inCount)
	{
	}
}
