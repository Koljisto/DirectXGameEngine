#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
#include <d3d11.h>
#include "ConstantBufferTypes.h"
#include <wrl/client.h>
#include "../ErrorLogger.h"

template <class T>
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	ID3D11DeviceContext* device_context_ = nullptr;

public:
	ConstantBuffer() {};
	T data_;
	ID3D11Buffer * Get() const
	{
		return buffer_.Get();
	}
	ID3D11Buffer * const* GetAddressOf() const
	{
		return buffer_.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
	{
		this->device_context_ = device_context;

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16));
		desc.StructureByteStride = 0;

		const HRESULT hr = device->CreateBuffer(&desc, 0, buffer_.GetAddressOf());
		return hr;
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		const HRESULT hr = this->device_context_->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
		if(FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to map constant buffer.");
			return false;
		}
		CopyMemory(mapped_subresource.pData, &data_, sizeof(T));
		this->device_context_->Unmap(buffer_.Get(), 0);
		return true;
	}
};
#endif
