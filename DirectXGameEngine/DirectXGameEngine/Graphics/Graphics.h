#pragma once
#include "AdapterReader.h"
#include "Shaders.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame() const;
private:
	bool InitializeDirectX(HWND hwnd, int width, int height);
	bool InitializeShaders();
		
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_;

	VertexShader vertex_shader_;
	PixelShader pixel_shader_;
};
