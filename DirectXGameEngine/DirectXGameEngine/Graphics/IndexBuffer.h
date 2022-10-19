#ifndef IndicesBuffer_h__
#define IndicesBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class IndexBuffer
{
private:
	IndexBuffer(const IndexBuffer& rhs);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	UINT buffer_size_ = 0;
public:
	IndexBuffer() {}

	ID3D11Buffer* Get() const
	{
		return buffer_.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return buffer_.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return this->buffer_size_;
	}

	HRESULT Initialize(ID3D11Device *device, DWORD * data, const UINT num_indices)
	{
		this->buffer_size_ = num_indices;
		// Load Index Data
		D3D11_BUFFER_DESC index_buffer_desc;
		ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));
		index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		index_buffer_desc.ByteWidth = sizeof(DWORD) * num_indices;
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.CPUAccessFlags = 0;
		index_buffer_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA index_buffer_data;
		index_buffer_data.pSysMem = data;
		const HRESULT hr = device->CreateBuffer(&index_buffer_desc, &index_buffer_data, buffer_.GetAddressOf());
		return hr;
	}
};

#endif
