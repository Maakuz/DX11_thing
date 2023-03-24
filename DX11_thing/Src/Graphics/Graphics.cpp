#include "Graphics.h"

bool Graphics::initialize(HWND hwnd, int width, int height)
{
    if (!this->initializeDirectX(hwnd, width, height))
        return false;

    if (!this->initializeShaders())
        return false;

    if (!this->initializeScene())
        return false;

    return true;
}

void Graphics::render()
{
    float l_bgCol[] = {0.f, 0.f, 1.f, 1.f};
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), l_bgCol);

    m_deviceContext->IASetInputLayout(m_vertexShader.getInputLayout());
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_deviceContext->VSSetShader(m_vertexShader.getShader(), NULL, 0);
    m_deviceContext->PSSetShader(m_pixelShader.getShader(), NULL, 0);

    UINT l_stride = sizeof(Vertex);
    UINT l_offset = 0;

    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &l_stride, &l_offset);

    m_deviceContext->Draw(3, 0); //No hardcoded num

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

    //Viewport
    D3D11_VIEWPORT l_viewPort = {0};

    l_viewPort.TopLeftX = 0;
    l_viewPort.TopLeftY = 0;
    l_viewPort.Width = width;
    l_viewPort.Height = height;

    m_deviceContext->RSSetViewports(1, &l_viewPort);

    return true;
}

bool Graphics::initializeShaders()
{
    std::string l_shaderFolder = "";
    if (IsDebuggerPresent() == TRUE)
    {
#ifdef _DEBUG
    #ifdef _WIN64
        l_shaderFolder = "../x64/Debug/";
    #else 
        l_shaderFolder = "../Debug/";
    #endif // _WIN64
#else
    #ifdef _WIN64
        l_shaderFolder = "../x64/Release/";
    #else
        l_shaderFolder = "../Release/";
    #endif // _WIN64
#endif // _DEBUG
    }

    D3D11_INPUT_ELEMENT_DESC l_layout[] =
    {
        {"POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT l_elementCount = ARRAYSIZE(l_layout);

    if (!m_vertexShader.initialize(m_device, l_shaderFolder + "vertexshader.cso", l_layout, l_elementCount))
        return false;

    if (!m_pixelShader.initialize(m_device, l_shaderFolder + "pixelshader.cso"))
        return false;

    return true;
}

bool Graphics::initializeScene()
{
    Vertex l_v[] =
    {
        {0.f, 0.3f, 
        1.f, 0.f, 0.f},

        {0.3, -0.3,
        0.f, 1.f, 0.f},

        {-0.3, -0.3,
        0.f, 0.f, 1.f},
    };

    D3D11_BUFFER_DESC l_vertexBufferDesc = {0};

    l_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    l_vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(l_v);
    l_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    l_vertexBufferDesc.CPUAccessFlags = 0;
    l_vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA l_vertexBufferData = {0};
    l_vertexBufferData.pSysMem = l_v;

    HRESULT l_hr = m_device->CreateBuffer(&l_vertexBufferDesc, &l_vertexBufferData, m_vertexBuffer.GetAddressOf());

    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Vertex buffer creation failed.");
        return false;
    }

    return true;
}
