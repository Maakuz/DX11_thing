#include "AdapterReader.h"
std::vector<AdapterData> AdapterReader::m_adapters;

std::vector<AdapterData> AdapterReader::getAdapters()
{
    if (m_adapters.size() > 0)
        return m_adapters;

    Microsoft::WRL::ComPtr<IDXGIFactory> l_pFactory;

    HRESULT l_hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)l_pFactory.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "DXGIFactory creation failed.");
        exit(-1);
    }

    IDXGIAdapter* l_pAdapter;
    UINT l_index = 0;
    while (SUCCEEDED(l_pFactory->EnumAdapters(l_index, &l_pAdapter)))
    {
        m_adapters.push_back(AdapterData(l_pAdapter));
        l_index++;
    }

    return m_adapters;
}

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
    m_pAdapter = pAdapter;
    HRESULT hr = pAdapter->GetDesc(&m_description);
    if (FAILED(hr))
    {
        ErrorLogger::log(hr, "Failed to get desc for IXGIAdapter");
    }
}
