#include "Hash.h"

namespace ShaderSystem
{
	uint64_t HashString(const std::string& str)
	{
		uint32_t i;
		uint64_t hash = 2166136261UL;
		unsigned char* p = (unsigned char*)str.c_str();

		for (i = 0; i < str.size(); i++)
			hash = (hash ^ p[i]) * 16777619;

		return hash;
	}
}
