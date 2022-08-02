#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters_;

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	if (!adapters_.empty()) // If already initialized
		return adapters_;

	Microsoft::WRL::ComPtr<IDXGIFactory> p_factory;

	const HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(p_factory.GetAddressOf()));
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters.");
		exit(-1);
	}
	IDXGIAdapter* p_adapter;
	UINT index = 0;
	while(SUCCEEDED(p_factory->EnumAdapters(index, &p_adapter)))
	{
		adapters_.emplace_back(p_adapter);
		index += 1;
	}
	return adapters_;
}

AdapterData::AdapterData(IDXGIAdapter* p_adapter)
{
	this->p_adapter_ = p_adapter;
	const HRESULT hr = p_adapter->GetDesc(&this->description_);
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to Get Description for IDXGIAdapter.");
	}
}

