#include "Shaders.h"

bool VertexShader::initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT elementCount)
{
    HRESULT l_hr = D3DReadFileToBlob(StringConverter::stringToWide(shaderPath).c_str(), m_shaderBuffer.GetAddressOf());

    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed to read shader: " + shaderPath);
        return false;
    }

    l_hr = device->CreateVertexShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed to create vertex shader: " + shaderPath);
        return false;
    }

    l_hr = device->CreateInputLayout(layoutDesc, elementCount, m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), m_inputLayout.GetAddressOf());

    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Input layout creation failed for: " + shaderPath);
        return false;
    }

    return true;
}

bool PixelShader::initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::string shaderPath)
{
    HRESULT l_hr = D3DReadFileToBlob(StringConverter::stringToWide(shaderPath).c_str(), m_shaderBuffer.GetAddressOf());

    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed to read shader: " + shaderPath);
        return false;
    }

    l_hr = device->CreatePixelShader(m_shaderBuffer->GetBufferPointer(), m_shaderBuffer->GetBufferSize(), NULL, m_shader.GetAddressOf());
    if (FAILED(l_hr))
    {
        ErrorLogger::log(l_hr, "Failed to create vertex shader: " + shaderPath);
        return false;
    }

    return true;
}
