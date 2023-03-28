#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "ConstantBufferStructs.h"
#include "../ErrorLogger.h"

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	ID3D11DeviceContext* m_deviceContext = nullptr;

public:
	T m_data;

	ConstantBuffer() {}

	ID3D11Buffer* Get() const { return m_buffer.Get(); }

	ID3D11Buffer* const* GetAddressOf() const { return m_buffer.GetAddressOf(); }

	bool UpdateBuffer()
	{
		D3D11_MAPPED_SUBRESOURCE l_subRes;
		HRESULT l_hr = m_deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &l_subRes);
		if (FAILED(l_hr))
		{
			ErrorLogger::log("Failed to map constant buffer.");
			return false;
		}

		CopyMemory(l_subRes.pData, &m_data, sizeof(T));
		m_deviceContext->Unmap(m_buffer.Get(), 0);

		return true;
	}

	HRESULT initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		if (m_buffer.Get() != nullptr)
			m_buffer.Reset();

		m_deviceContext = deviceContext;

		D3D11_BUFFER_DESC l_ConstantBufferDesc = { 0 };

		l_ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		l_ConstantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		l_ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		l_ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		l_ConstantBufferDesc.MiscFlags = 0;
		l_ConstantBufferDesc.StructureByteStride = 0;

		HRESULT l_hr = device->CreateBuffer(&l_ConstantBufferDesc, 0, m_buffer.GetAddressOf());

		return l_hr;
	}
};