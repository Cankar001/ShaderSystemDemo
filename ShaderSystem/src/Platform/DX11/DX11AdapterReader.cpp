#include "DX11AdapterReader.h"

#include "Core/Logger.h"

namespace ShaderSystem
{
	std::vector<AdapterData> AdapterReader::sAdapters;

	AdapterData::AdapterData(IDXGIAdapter *adapter)
	{
		mAdapter = adapter;
		HRESULT result = adapter->GetDesc(&mDescription);
		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to get adapter description");
	}

	std::vector<AdapterData> AdapterReader::GetAdapters()
	{
		if (sAdapters.size() > 0)
			return sAdapters;

		ComPtr<IDXGIFactory> factory;
		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(factory.GetAddressOf()));

		if (FAILED(result))
			SHADER_SYSTEM_ERROR("Failed to create DXGIFactory");

		IDXGIAdapter *adapter;
		UINT index = 0;
		while (SUCCEEDED(factory->EnumAdapters(index, &adapter)))
		{
			sAdapters.push_back(AdapterData(adapter));
			index++;
		}

		return sAdapters;
	}
}
