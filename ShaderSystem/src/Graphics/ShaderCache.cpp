#include "ShaderCache.h"

#include "Core/Hash.h"

#include <map>

namespace ShaderSystem
{
	static std::map<std::string, uint64_t> sShaderCache;

	static void Serialize(const std::map<std::string, uint64_t>& inShaderCache);
	static void Deserialize(std::map<std::string, uint64_t>& inShaderCache);

	void ShaderCache::Init()
	{
		Deserialize(sShaderCache);
	}

	void ShaderCache::Shutdown()
	{
		Serialize(sShaderCache);
	}

	bool ShaderCache::HasChanged(const std::filesystem::path& inFilePath, const std::string& inSource)
	{
		uint64_t hash = HashString(inSource);

		if (sShaderCache.find(inFilePath.string()) == sShaderCache.end() || sShaderCache.at(inFilePath.string()) != hash)
		{
			sShaderCache[inFilePath.string()] = hash;
			return true;
		}

		return false;
	}

	static void Serialize(const std::map<std::string, uint64_t>& inShaderCache)
	{
		// TODO: implement
	}

	static void Deserialize(std::map<std::string, uint64_t>& inShaderCache)
	{
		// TODO: implement
	}
}
