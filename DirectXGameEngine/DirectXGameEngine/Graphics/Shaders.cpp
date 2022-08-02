#include "Shaders.h"

bool VertexShader::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& shader_path,
                              const D3D11_INPUT_ELEMENT_DESC* layout_desc, const UINT num_elements)
{
	HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), this->shader_buffer_.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring error_msg = L"Failed to load shader: ";
		error_msg += shader_path;
		ErrorLogger::Log(hr, error_msg);
		return false;
	}

	hr = device->CreateVertexShader(this->shader_buffer_.Get()->GetBufferPointer(), this->shader_buffer_.Get()->GetBufferSize(), NULL, this->shader_.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring error_msg = L"Failed to create vertex shader: ";
		error_msg += shader_path;
		ErrorLogger::Log(hr, error_msg);
		return false;
	}

	hr = device->CreateInputLayout(layout_desc, num_elements, this->shader_buffer_->GetBufferPointer(), this->shader_buffer_->GetBufferSize(), this->input_layout_.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create input layout.");
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader() const
{
	return this->shader_.Get();
}

ID3D10Blob* VertexShader::GetBuffer() const
{
	return this->shader_buffer_.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout() const
{
	return this->input_layout_.Get();
}

bool PixelShader::Initialize(const Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::wstring& shader_path)
{
	HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), this->shader_buffer_.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring error_msg = L"Failed to load shader: ";
		error_msg += shader_path;
		ErrorLogger::Log(hr, error_msg);
		return false;
	}

	hr = device->CreatePixelShader(this->shader_buffer_.Get()->GetBufferPointer(), this->shader_buffer_.Get()->GetBufferSize(), NULL, this->shader_.GetAddressOf());
	if (FAILED(hr))
	{
		std::wstring error_msg = L"Failed to create pixel shader: ";
		error_msg += shader_path;
		ErrorLogger::Log(hr, error_msg);
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader() const
{
	return this->shader_.Get();
}

ID3D10Blob* PixelShader::GetBuffer() const
{
	return this->shader_buffer_.Get();
}
