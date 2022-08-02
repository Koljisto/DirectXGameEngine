#pragma once
#include "..//ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <wrl//client.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device> &device, const std::wstring& shader_path, const D3D11_INPUT_ELEMENT_DESC* layout_desc, UINT num_elements);
	ID3D11VertexShader* GetShader() const;
	ID3D10Blob* GetBuffer() const;
	ID3D11InputLayout* GetInputLayout() const;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader_;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer_;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_;
};

class PixelShader
{
public:
	bool Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& shader_path);
	ID3D11PixelShader* GetShader() const;
	ID3D10Blob* GetBuffer() const;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
	Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer_;
};
