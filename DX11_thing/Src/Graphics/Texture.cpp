#include "Texture.h"
#include "../ErrorLogger.h"

Texture::Texture(ID3D11Device* device, const Color& col, TextureType type)
{
    this->initialize1x1ColorTexture(device, col, type);
}

Texture::Texture(ID3D11Device* device, const Color& col, UINT width, UINT height, TextureType type)
{
    this->initializeColorTexture(device, col, width, height, type);
}

Texture::Texture(const Texture& cop)
{
    m_texture = cop.m_texture;
    m_textureView = cop.m_textureView;
    m_type = cop.m_type;
}

Texture& Texture::operator=(const Texture& cop)
{
    m_texture = cop.m_texture;
    m_textureView = cop.m_textureView;
    m_type = cop.m_type;
    return *this;
}

void Texture::initialize1x1ColorTexture(ID3D11Device* device, const Color& col, TextureType type)
{
    this->initializeColorTexture(device, col, 1, 1, type);
}

void Texture::initializeColorTexture(ID3D11Device* device, const Color& col, UINT width, UINT height, TextureType type)
{
    m_type = type;
    CD3D11_TEXTURE2D_DESC l_desc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
    ID3D11Texture2D* l_texture = nullptr;
    D3D11_SUBRESOURCE_DATA l_initialData = { 0 };
    l_initialData.pSysMem = &col;
    l_initialData.SysMemPitch = width * sizeof(Color);
    
    HRESULT l_hr = device->CreateTexture2D(&l_desc, &l_initialData, &l_texture);
    COM_ERROR_IF_FAILED(l_hr, "Failed to initialize Texture from color");

    m_texture = static_cast<ID3D11Texture2D*>(l_texture);
    CD3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc(D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D, l_desc.Format);
    device->CreateShaderResourceView(m_texture.Get(), &l_srvDesc, m_textureView.GetAddressOf());
    COM_ERROR_IF_FAILED(l_hr, "Failed to create shader resource view");
}
