#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>
class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_stride = sizeof(T);
	UINT m_vertexCount = 0;

public:
	VertexBuffer() {}

	VertexBuffer<T>& operator=(const VertexBuffer<T>& cop)
	{
		m_buffer = cop.m_buffer;
		m_vertexCount = cop.m_vertexCount;
		m_stride = cop.m_stride;
		return *this;
	}

	VertexBuffer(const VertexBuffer<T>& cop)
	{
		m_buffer = cop.m_buffer;
		m_vertexCount = cop.m_vertexCount;
		m_stride = cop.m_stride;
	}

	ID3D11Buffer* Get() const { return m_buffer.Get(); }

	ID3D11Buffer* const* GetAddressOf() const { return m_buffer.GetAddressOf(); }

	UINT VertexCount() const { return m_vertexCount; }

	const UINT Stride() const { return m_stride; }

	const UINT* StridePtr() const { return &m_stride; }

	HRESULT initialize(ID3D11Device* device, T* data, UINT vertexCount)
	{
		if (m_buffer.Get() != nullptr)
			m_buffer.Reset();

		m_vertexCount = vertexCount;

		D3D11_BUFFER_DESC l_vertexBufferDesc = { 0 };

		l_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		l_vertexBufferDesc.ByteWidth = m_stride * vertexCount;
		l_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_vertexBufferDesc.CPUAccessFlags = 0;
		l_vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA l_vertexBufferData = { 0 };
		l_vertexBufferData.pSysMem = data;

		HRESULT l_hr = device->CreateBuffer(&l_vertexBufferDesc, &l_vertexBufferData, m_buffer.GetAddressOf());

		return l_hr;
	}
};