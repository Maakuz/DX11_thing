#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_bufferSize = 0;

public:
	IndexBuffer() {}

	ID3D11Buffer* Get() const { return m_buffer.Get(); }

	ID3D11Buffer* const* GetAddressOf() const { return m_buffer.GetAddressOf(); }

	UINT IndexCount() const { return m_bufferSize; }

	HRESULT initialize(ID3D11Device* device, DWORD* data, UINT indexCount)
	{
        if (m_buffer.Get() != nullptr)
            m_buffer.Reset();

        m_bufferSize = indexCount;

        D3D11_BUFFER_DESC l_indexBufferDesc = { 0 };
        l_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        l_indexBufferDesc.ByteWidth = sizeof(DWORD) * indexCount;
        l_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        l_indexBufferDesc.CPUAccessFlags = 0;
        l_indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA l_indexBufferData;
        l_indexBufferData.pSysMem = data;
        HRESULT l_hr = device->CreateBuffer(&l_indexBufferDesc, &l_indexBufferData, m_buffer.GetAddressOf());
        return l_hr;
	}
};

