#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Color.h"

//TODO: Create material

enum class TextureStorageType
{
    Invalid, 
    None,
    EmbeddedIndexCompressed,
    EmbeddedIndexNonCompressed,
    EmbeddedCompressed,
    EmbeddedNonCompressed,
    Disk
};

//TODO: move to importer probably
enum class TextureType
{
    Unknown,
    Diffuse
};

class Texture
{
public:
    Texture(ID3D11Device* device, const Color& col, TextureType type);
    Texture(ID3D11Device* device, const Color& col, UINT width, UINT height, TextureType type);
    Texture(const Texture& cop);
    Texture& operator=(const Texture& cop);

    TextureType getType() { return m_type; }
    ID3D11ShaderResourceView* getTextureResourceView() { return m_textureView.Get(); }
    ID3D11ShaderResourceView** getTextureResourceViewAddress() { return m_textureView.GetAddressOf(); }

private:
    void initialize1x1ColorTexture(ID3D11Device* device, const Color& col, TextureType type);
    void initializeColorTexture(ID3D11Device* device, const Color& col, UINT width, UINT height, TextureType type);

    Microsoft::WRL::ComPtr<ID3D11Resource> m_texture = nullptr;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView = nullptr;
    TextureType m_type = TextureType::Unknown;
};