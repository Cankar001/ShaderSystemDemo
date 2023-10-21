#pragma once

// TODO: Test if this works, but should be fine.
#if 0

#include "Core/Core.h"
#include "ShaderPreProcessor.h"

#include <dxc/dxcapi.h>

namespace ShaderSystem
{
	class HLSLIncluder : public IDxcIncludeHandler
	{
	public:

		HRESULT LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource) override;
		HRESULT QueryInterface(const IID& riid, void** ppvObject) override
		{
			return sDefaultIncludeHandler->QueryInterface(riid, ppvObject);
		}

		ULONG AddRef() override { return 0; }
		ULONG Release() override { return 0; }

		std::unordered_set<IncludeData>&& GetIncludeData() { return std::move(mIncludeData); }
		std::unordered_set<std::string>&& GetParsedSpecialMacros() { return std::move(mParsedSpecialMacros); }

	private:

		inline static IDxcIncludeHandler* sDefaultIncludeHandler = nullptr;

		std::unordered_set<IncludeData> mIncludeData;
		std::unordered_set<std::string> mParsedSpecialMacros;
		std::unordered_map<std::string, HeaderCache> mHeaderCache;
	};
}

#endif 
