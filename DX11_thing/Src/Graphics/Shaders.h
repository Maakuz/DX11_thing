#pragma once
#include "../ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

//Todo: inheritance magic

class VertexShader
{
public:
    bool initialize(Microsoft::WRL::ComPtr<ID3D11Device> &device, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC * layoutDesc, UINT elementCount);
    ID3D11VertexShader* getShader() { return m_shader.Get(); };
    ID3D10Blob* getBuffer() { return m_shaderBuffer.Get(); };
    ID3D11InputLayout* getInputLayout() { return m_inputLayout.Get(); }

private:
    Microsoft::WRL::ComPtr <ID3D11VertexShader> m_shader = nullptr;
    Microsoft::WRL::ComPtr <ID3D10Blob> m_shaderBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;;
};

class PixelShader
{
public:
    bool initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::string shaderPath);
    ID3D11PixelShader* getShader() { return m_shader.Get(); };
    ID3D10Blob* getBuffer() { return m_shaderBuffer.Get(); };
private:
    Microsoft::WRL::ComPtr <ID3D11PixelShader> m_shader = nullptr;
    Microsoft::WRL::ComPtr <ID3D10Blob> m_shaderBuffer = nullptr;
};