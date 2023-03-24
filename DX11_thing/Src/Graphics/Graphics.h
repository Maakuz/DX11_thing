#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

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

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	VertexBuffer<Vertex> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
	ConstantBuffer<ConstantBuffers::VS_vertexshader> m_constantBuffer;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch; //TODO: SEPARATE TO TEXT CLASS?
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};