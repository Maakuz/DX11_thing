#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class VertexBuffer
{
private:
	VertexBuffer(const VertexBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	std::unique_ptr<UINT> m_stride;
	UINT m_bufferSize = 0;

public:
	VertexBuffer() {}

	ID3D11Buffer* Get() const { return m_buffer.Get(); }

	ID3D11Buffer* const* GetAddressOf() const { return m_buffer.GetAddressOf(); }

	UINT BufferSize() const { return m_bufferSize; }

	const UINT Stride() const { return *m_stride.Get(); }

	const UINT* StridePtr() const { return m_stride.get(); }

	HRESULT initialize(ID3D11Device* device, T* data, UINT vertexCount)
	{
		m_bufferSize = vertexCount;
		m_stride = std::make_unique<UINT>(sizeof(T));

		D3D11_BUFFER_DESC l_vertexBufferDesc = { 0 };

		l_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		l_vertexBufferDesc.ByteWidth = sizeof(T) * vertexCount;
		l_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_vertexBufferDesc.CPUAccessFlags = 0;
		l_vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA l_vertexBufferData = { 0 };
		l_vertexBufferData.pSysMem = data;

		HRESULT l_hr = device->CreateBuffer(&l_vertexBufferDesc, &l_vertexBufferData, m_buffer.GetAddressOf());

		return l_hr;
	}
};