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
    float l_bgCol[] = {0.3f, 0.3f, 0.3f, 1.f};
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), l_bgCol);
    m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    m_deviceContext->IASetInputLayout(m_vertexShader.getInputLayout());
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->RSSetState(m_rasterizerState.Get());
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

    m_deviceContext->VSSetShader(m_vertexShader.getShader(), NULL, 0);

    m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    m_deviceContext->PSSetShader(m_pixelShader.getShader(), NULL, 0);

    //CB 
    m_constantBuffer.m_data = { 0.5f, 0.5f };
    m_constantBuffer.UpdateBuffer();

    m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    UINT l_stride = sizeof(Vertex);
    UINT l_offset = 0;

    m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &l_offset);
    m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    m_deviceContext->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
    m_deviceContext->DrawIndexed(m_indexBuffer.IndexCount(), 0, 0); //TODO: No hardcoded num

    //text
    m_spriteBatch->Begin();
    m_spriteFont->DrawString(m_spriteBatch.get(), "Hello boy", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1, 1));
    m_spriteBatch->End();

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

    //Depth stencil
    D3D11_TEXTURE2D_DESC l_depthStencilDesc;
    l_depthStencilDesc.Width = width;
    l_depthStencilDesc.Height = height;
    l_depthStencilDesc.MipLevels = 1;
    l_depthStencilDesc.ArraySize = 1;
    l_depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    l_depthStencilDesc.SampleDesc.Count = 1;
    l_depthStencilDesc.SampleDesc.Quality = 0;
    l_depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    l_depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    l_depthStencilDesc.CPUAccessFlags = 0;
    l_depthStencilDesc.MiscFlags = 0;

    l_hr = m_device->CreateTexture2D(&l_depthStencilDesc, NULL, m_depthStencilBuffer.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed creating depth texture.");
        return false;
    }

    l_hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), NULL, m_depthStencilView.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed creating depth stencil view.");
        return false;
    }

    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    //Depth stencil state
    D3D11_DEPTH_STENCIL_DESC l_depthStencilStateDesc = { 0 };
    l_depthStencilStateDesc.DepthEnable = true;
    l_depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    l_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    l_hr = m_device->CreateDepthStencilState(&l_depthStencilStateDesc, m_depthStencilState.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed creating depth stencil state.");
        return false;
    }

    //Viewport
    D3D11_VIEWPORT l_viewPort = {0};

    l_viewPort.TopLeftX = 0;
    l_viewPort.TopLeftY = 0;
    l_viewPort.Width = width;
    l_viewPort.Height = height;
    l_viewPort.MaxDepth = 0.f;
    l_viewPort.MaxDepth = 1.f;

    m_deviceContext->RSSetViewports(1, &l_viewPort);

    D3D11_RASTERIZER_DESC l_rasterDesc;
    ZeroMemory(&l_rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

    l_rasterDesc.FillMode = D3D11_FILL_SOLID; //Wireframe possible
    l_rasterDesc.CullMode = D3D11_CULL_BACK;
    l_hr = m_device->CreateRasterizerState(&l_rasterDesc, m_rasterizerState.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed creating raster state.");
        return false;
    }

    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());
    m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"Data/Fonts/Comic_sans16.spritefont");

    //Sampler
    D3D11_SAMPLER_DESC l_sampDesc;
    ZeroMemory(&l_sampDesc, sizeof(D3D11_SAMPLER_DESC));

    l_sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    l_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    l_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    l_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    l_sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    l_sampDesc.MinLOD = 0;
    l_sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    l_hr = m_device->CreateSamplerState(&l_sampDesc, m_samplerState.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed creating sampler state.");
        return false;
    }

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
        {"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
        {-0.3f, 0.3f, 0.5f, 
        0.f, 0.f},

        {0.3, 0.3, 0.5f,
        1.f, 0.f},

        {0.3, -0.3, 0.5f,
        1.f, 1.f},

        {-0.3, -0.3, 0.5f,
        0.f, 1.f},
    };

    DWORD l_indicies[] = 
    {
        0, 1, 2, 
        2, 3, 0
    };

    HRESULT l_hr = m_vertexBuffer.initialize(m_device.Get(), l_v, ARRAYSIZE(l_v));
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Vertex buffer creation failed.");
        return false;
    }

    l_hr = m_indexBuffer.initialize(m_device.Get(), l_indicies, 6);
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "index buffer creation failed.");
        return false;
    }

    //Texture setup
    l_hr = DirectX::CreateWICTextureFromFile(m_device.Get(), L"Data/Textures/julpeg.png", nullptr, m_texture.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Texture load failed.");
        return false;
    }

    l_hr =  m_constantBuffer.initialize(m_device.Get(), m_deviceContext.Get());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed to create CB.");
        return false;
    }

    return true;
}
