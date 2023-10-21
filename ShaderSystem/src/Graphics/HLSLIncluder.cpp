#include "HLSLIncluder.h"

// TODO: Test if this works, but should be fine.
#if 0

#include "Core/Assert.h"
#include "Core/Hash.h"
#include "Utils/ShaderIncluderUtils.h"

namespace ShaderSystem
{
	HRESULT HLSLIncluder::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
	{
		static IDxcUtils* pUtils = nullptr;
		if (!pUtils)
		{
			DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
			HRESULT result = pUtils->CreateDefaultIncludeHandler(&sDefaultIncludeHandler);
			SHADER_SYSTEM_ASSERT(!FAILED(result), "Failed to create default include handler!");
		}

		const std::filesystem::path filePath = pFilename;
		auto& [source, sourceHash, stages, isGuarded] = mHeaderCache[filePath.string()];

		if (source.empty())
		{
			source = utils::ReadFileAndSkipBOM(filePath);
			if (source.empty())
			{
				// Note: No error logging because dxc tries multiple include directories with the same file until it finds it.
				*ppIncludeSource = nullptr;
				return S_FALSE;
			}

			sourceHash = HashString(source);

			// Can clear "source" in case it has already been included in this stage.
			stages = ShaderPreProcessor::PreprocessHeader<ShaderLanguage::HLSL>(source, isGuarded, mParsedSpecialMacros, mIncludeData, filePath);
		}
		else if (isGuarded)
		{
			source.clear();
		}

		// TODO: Get real values for IncludeDepth and IsRelative?
		mIncludeData.emplace(IncludeData{ filePath, 0, false, isGuarded, sourceHash, stages });

		IDxcBlobEncoding* pEncoding;
		pUtils->CreateBlob(source.data(), (uint32_t)source.size(), CP_UTF8, &pEncoding);

		*ppIncludeSource = pEncoding;
		return S_OK;
	}
}

#endif
