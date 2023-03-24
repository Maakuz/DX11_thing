#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"

class Graphics
{
public:
	bool initialize(HWND hwnd, int width, int height);

	void render();

private:
	bool initializeDirectX(HWND hwnd, int width, int height);
	bool initializeShaders();
	bool initializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};