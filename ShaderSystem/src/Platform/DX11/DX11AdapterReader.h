#pragma once

#include "Core/Core.h"

#include "DX11Resources.h"

namespace ShaderSystem
{
	class AdapterData
	{
	public:

		AdapterData(IDXGIAdapter *adapter);

		IDXGIAdapter *mAdapter;
		DXGI_ADAPTER_DESC mDescription;
	};

	class AdapterReader
	{
	public:

		static std::vector<AdapterData> GetAdapters();

	private:

		static std::vector<AdapterData> sAdapters;
	};
}
