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
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
private:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool InitializeShaders();
	bool InitializeScene();
		
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_;

	VertexShader vertex_shader_;
	PixelShader pixel_shader_;
	ConstantBuffer<CB_VS_vertex_shader> constant_buffer_;

	VertexBuffer<Vertex> vertex_buffer_;
	IndexBuffer indices_buffer_;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer_;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state_;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state_;

	std::unique_ptr<DirectX::SpriteBatch> sprite_batch_;
	std::unique_ptr<DirectX::SpriteFont> sprite_font_;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sample_state_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;
};
