#include "Graphics.h"

bool Graphics::initialize(HWND hwnd, int width, int height)
{
    if (!this->initializeDirectX(hwnd, width, height))
        return false;

    return true;
}

void Graphics::render()
{
    float l_bgCol[] = {0.f, 0.f, 1.f, 1.f};
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), l_bgCol);

    m_swapChain->Present(1, NULL);
}

bool Graphics::initializeDirectX(HWND hwnd, int width, int height)
{
    std::vector<AdapterData> l_adapters = AdapterReader::getAdapters();

    if (l_adapters.size() < 1)
    {
        ErrorLogger::log("No DXGI adapters found.");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC l_scd = { 0 };

    l_scd.BufferDesc.Width = width;
    l_scd.BufferDesc.Height = height;
    l_scd.BufferDesc.RefreshRate.Numerator = 120;
    l_scd.BufferDesc.RefreshRate.Denominator = 1;
    l_scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    l_scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    l_scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    l_scd.SampleDesc.Count = 1;
    l_scd.SampleDesc.Quality = 0;

    l_scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    l_scd.BufferCount = 1;
    l_scd.OutputWindow = hwnd;
    l_scd.Windowed = true;
    l_scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    l_scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


    HRESULT l_hr = D3D11CreateDeviceAndSwapChain(
        l_adapters[0].m_pAdapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,   //SOFTWARE DRIVER TYPE
        NULL,   //RUNTIME LAYERS
        NULL,   //FEATURE LEVELS ARRAY
        0,      //FEATURE LEVELS AMOUNT
        D3D11_SDK_VERSION,
        &l_scd,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        NULL, //SUPPORTED FEATURE LEVELS
        m_deviceContext.GetAddressOf());

    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed to create device and swap chain.");
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> l_backBuffer;
    l_hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(l_backBuffer.GetAddressOf()));
    
    if (FAILED(l_hr)) 
    {
        ErrorLogger::log(l_hr, "GetBuffer failed");
        return false;
    }

    l_hr = m_device->CreateRenderTargetView(l_backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf());

    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "CreateRenderTargetView failed");
        return false;
    }

    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);

    return true;
}
