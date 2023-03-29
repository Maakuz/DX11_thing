#include "Graphics.h"
#include "../ConsoleWindow.h"

bool Graphics::initialize(HWND hwnd, int width, int height)
{
    m_winHeight = height;
    m_winWidth = width;

    m_timer.start();

    if (!this->initializeDirectX(hwnd))
        return false;

    if (!this->initializeShaders())
        return false;

    if (!this->initializeScene())
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
    ImGui::StyleColorsDark();
    
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
    //m_deviceContext->OMSetBlendState(m_blendstateState.Get(), NULL, 0xFFFFFFFF);
    m_deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
    m_deviceContext->PSSetShader(m_pixelShader.getShader(), NULL, 0);

    static float xOffset = 0.f;
    monitorCon.addDragFloat("xOffset", &xOffset, 0.01f);

    static float zRot = 0.f;
    monitorCon.addDragFloat("zRot", &zRot, 0.01f);

    m_object.setPos(xOffset, 0, 0);
    m_object.setRotation(0, 0, zRot);

    m_object.draw(m_camera.getView() * m_camera.getProjection());

    //text //TODO: MAKE BETTER
    static int s_fps = 0;
    static std::string s_fpsStr;
    s_fps++;

    if (m_timer.millisecondsElapsed() > 1000)
    {
        s_fpsStr = std::to_string(s_fps);
        monitorCon.addString("FPS", &s_fpsStr);
        s_fps = 0;
        m_timer.restart();
    }


    //m_spriteBatch->Begin();
    //m_spriteFont->DrawString(m_spriteBatch.get(), s_fpsStr.c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1, 1));
    //m_spriteBatch->End();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ConsoleWindow::get().update();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_swapChain->Present(1, NULL);
}

Graphics::~Graphics()
{
    if (ImGui::GetCurrentContext())
    {
        ImGui_ImplWin32_Shutdown();
        ImGui_ImplDX11_Shutdown();
        ImGui::DestroyContext();
    }
}

bool Graphics::initializeDirectX(HWND hwnd)
{
    try {
        std::vector<AdapterData> l_adapters = AdapterReader::getAdapters();

        if (l_adapters.size() < 1)
        {
            ErrorLogger::log("No DXGI adapters found.");
            return false;
        }

        DXGI_SWAP_CHAIN_DESC l_scd = { 0 };

        l_scd.BufferDesc.Width = m_winWidth;
        l_scd.BufferDesc.Height = m_winHeight;
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

        COM_ERROR_IF_FAILED(l_hr, "Failed to create device and swap chain.");

        Microsoft::WRL::ComPtr<ID3D11Texture2D> l_backBuffer;
        l_hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(l_backBuffer.GetAddressOf()));
        COM_ERROR_IF_FAILED(l_hr, "GetBuffer failed");

        l_hr = m_device->CreateRenderTargetView(l_backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "CreateRenderTargetView failed");

        //Depth stencil
        CD3D11_TEXTURE2D_DESC l_depthStencilDesc(
            DXGI_FORMAT_D24_UNORM_S8_UINT, 
            m_winWidth, 
            m_winHeight, 
            1, 
            1, 
            D3D11_BIND_DEPTH_STENCIL);

        l_hr = m_device->CreateTexture2D(&l_depthStencilDesc, NULL, m_depthStencilBuffer.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Failed creating depth texture");

        l_hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), NULL, m_depthStencilView.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Failed creating depth stencil view.");

        m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

        //Depth stencil state
        CD3D11_DEPTH_STENCIL_DESC l_depthStencilStateDesc(D3D11_DEFAULT);
        l_depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        l_hr = m_device->CreateDepthStencilState(&l_depthStencilStateDesc, m_depthStencilState.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Failed creating depth stencil state.");

        //Viewport
        CD3D11_VIEWPORT l_viewPort(0.f, 0.f, float(m_winWidth), float(m_winHeight));
        m_deviceContext->RSSetViewports(1, &l_viewPort);

        CD3D11_RASTERIZER_DESC l_rasterDesc(D3D11_DEFAULT); //Wireframe can be set here

        l_hr = m_device->CreateRasterizerState(&l_rasterDesc, m_rasterizerState.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Failed creating raster state.");

        //Blendstate
        D3D11_BLEND_DESC l_blendDesc = { 0 };
        D3D11_RENDER_TARGET_BLEND_DESC l_renderTargetBlendDesc = { 0 };

        l_renderTargetBlendDesc.BlendEnable = true;
        l_renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        l_renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        l_renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
        l_renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
        l_renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
        l_renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        l_renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        l_blendDesc.RenderTarget[0] = l_renderTargetBlendDesc;

        l_hr = m_device->CreateBlendState(&l_blendDesc, m_blendstateState.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Failed creating blend state");

        m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());
        m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"Data/Fonts/Comic_sans16.spritefont");

        //Sampler
        CD3D11_SAMPLER_DESC l_sampDesc(D3D11_DEFAULT);
        l_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        l_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        l_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        l_hr = m_device->CreateSamplerState(&l_sampDesc, m_samplerState.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Failed creating sampler state.");
    }
    catch (COMException& e)
    {
        ErrorLogger::log(e);
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
    try
    {
        HRESULT l_hr = DirectX::CreateWICTextureFromFile(m_device.Get(), L"Data/Textures/grass.jpeg", nullptr, m_textureGrass.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Texture load failed.");

        l_hr = DirectX::CreateWICTextureFromFile(m_device.Get(), L"Data/Textures/stone.jpeg", nullptr, m_texturePavement.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Texture load failed.");

        l_hr = DirectX::CreateWICTextureFromFile(m_device.Get(), L"Data/Textures/julpeg.png", nullptr, m_textureMissing.GetAddressOf());
        COM_ERROR_IF_FAILED(l_hr, "Texture load failed.");

        l_hr = m_constantVertexBuffer.initialize(m_device.Get(), m_deviceContext.Get());
        COM_ERROR_IF_FAILED(l_hr, "Failed to create CB.");

        l_hr = m_constantPixelBuffer.initialize(m_device.Get(), m_deviceContext.Get());
        COM_ERROR_IF_FAILED(l_hr, "VFailed to create CB.");

        m_object.initialize("Data/Models/cube.obj", m_device.Get(), m_deviceContext.Get(), m_constantVertexBuffer);

        m_camera.setPos(0, 0, -2);
        m_camera.setProjecton(90, float(m_winWidth) / float(m_winHeight), 0.1f, 1000.f);
    }
    
    catch (COMException& e)
    {
        ErrorLogger::log(e);
        return false;
    }

    return true;
}
