#include "Graphics.h"

bool Graphics::Initialize(const HWND hwnd, const int width, const int height)
{
	if (!InitializeDirectX(hwnd, width, height))
		return false;

	if (!InitializeShaders())
		return false;

	if (!InitializeScene())
		return false;

	return true;
}

void Graphics::RenderFrame()
{
	constexpr float bg_color[] = {0.f, 0.f, 0.f, 1.0f};
	this->device_context_->ClearRenderTargetView(this->render_target_view_.Get(), bg_color);
	this->device_context_->ClearDepthStencilView(this->depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->device_context_->IASetInputLayout(this->vertex_shader_.GetInputLayout());
	this->device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->device_context_->RSSetState(this->rasterizer_state_.Get());
	this->device_context_->OMSetDepthStencilState(this->depth_stencil_state_.Get(), 0);
	this->device_context_->PSSetSamplers(0, 1, this->sample_state_.GetAddressOf());
	this->device_context_->VSSetShader(vertex_shader_.GetShader(), NULL, 0);
	this->device_context_->PSSetShader(pixel_shader_.GetShader(), NULL, 0);

	UINT offset;
	offset = 0;

	// Update Constant Buffer
	constant_buffer_.data_.mat = DirectX::XMMatrixTranslation(0.0f, -0.5f, 0.0f);
	constant_buffer_.data_.mat = DirectX::XMMatrixTranspose(constant_buffer_.data_.mat);
	if (!constant_buffer_.ApplyChanges())
		return;
	this->device_context_->VSSetConstantBuffers(0, 1, this->constant_buffer_.GetAddressOf());
	// Square
	this->device_context_->PSSetShaderResources(0, 1, this->texture_.GetAddressOf());
	this->device_context_->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), vertex_buffer_.StridePtr(), &offset);
	this->device_context_->IASetIndexBuffer(indices_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

	this->device_context_->DrawIndexed(indices_buffer_.BufferSize(), 0, 0);

	// Draw Text
	sprite_batch_->Begin();
	sprite_font_->DrawString(sprite_batch_.get(), L"HELLO WORLD", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	sprite_batch_->End();

	this->swapchain_->Present(1, NULL);
}

bool Graphics::InitializeDirectX(const HWND hwnd, const int width, const int height)
{
	const std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if(adapters.empty())
	{
		ErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}
	AdapterData best_adapter = adapters[0];
	for (const auto& adapter : adapters)
	{
		if (adapter.description_.DedicatedVideoMemory > best_adapter.description_.DedicatedVideoMemory)
		{
			best_adapter = adapter;
		}
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(best_adapter.p_adapter_, // IDXGI Adapter
	                                           D3D_DRIVER_TYPE_UNKNOWN, // Unknown device driver
	                                           NULL, // For software driver type
	                                           NULL, // Flags for runtime layers
	                                           NULL, // Feature levels array
	                                           0, // # of feature levels in array 
	                                           D3D11_SDK_VERSION, // Default
	                                           &scd, // Swapchain description
	                                           this->swapchain_.GetAddressOf(), // Swapchain address
	                                           this->device_.GetAddressOf(), // Device address
	                                           NULL, // Supported feature level
	                                           this->device_context_.GetAddressOf());	// DeviceContext address
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	hr = this->swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
	if(FAILED(hr)) // If error occurred
	{
		ErrorLogger::Log(hr, "GetBuffer Failed.");
		return false;
	}

	hr = this->device_->CreateRenderTargetView(back_buffer.Get(), NULL, this->render_target_view_.GetAddressOf());

	if (FAILED(hr)) // If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	// Describe out Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC stencil_desc;
	stencil_desc.Width = width;
	stencil_desc.Height = height;
	stencil_desc.MipLevels = 1;
	stencil_desc.ArraySize = 1;
	stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencil_desc.SampleDesc.Count = 1;
	stencil_desc.SampleDesc.Quality = 0;
	stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	stencil_desc.CPUAccessFlags = 0;
	stencil_desc.MiscFlags = 0;

	hr = this->device_->CreateTexture2D(&stencil_desc, NULL, this->depth_stencil_buffer_.GetAddressOf());
	if(FAILED(hr)) // If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
		return false;
	}

	hr = this->device_->CreateDepthStencilView(this->depth_stencil_buffer_.Get(), NULL, this->depth_stencil_view_.GetAddressOf());
	if(FAILED(hr)) // If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil view.");
		return false;
	}

	this->device_context_->OMSetRenderTargets(1, this->render_target_view_.GetAddressOf(), this->depth_stencil_view_.Get());

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device_->CreateDepthStencilState(&depth_stencil_desc, this->depth_stencil_state_.GetAddressOf());
	if(FAILED(hr)) // If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil state.");
		return false;
	}

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Set the Viewport
	this->device_context_->RSSetViewports(1, &viewport);

	// Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	// Testing
	//rasterizer_desc.FrontCounterClockwise = TRUE;
	hr = this->device_->CreateRasterizerState(&rasterizer_desc, this->rasterizer_state_.GetAddressOf());
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state");
		return false;
	}

	sprite_batch_ = std::make_unique<DirectX::SpriteBatch>(this->device_context_.Get());
	sprite_font_ = std::make_unique<DirectX::SpriteFont>(this->device_.Get(), L"Data/Fonts/comic_sans_ms_16.spritefont");

	// Create sampler description for sampler state
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = this->device_->CreateSamplerState(&sampler_desc, this->sample_state_.GetAddressOf());
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create sampler state.");
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders()
{
	std::wstring shader_folder;

#pragma region DetermineShaderPath
	if(IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG // Debug mode
	#ifdef _WIN64 // x64
		shader_folder = L"../x64/Debug/";
	#else // x86
		shader_folder = L"../Debug/";
	#endif
#else // Release mode
	#ifdef _WIN64 // x64
		shader_folder = L"../x64/Release/";
	#else // x86
		shader_folder = L"../Release/";
	#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT, // R32-G32-B32
			0,
			0,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	constexpr UINT num_elements = ARRAYSIZE(layout);

	if (!vertex_shader_.Initialize(this->device_, shader_folder + L"VertexShader.cso", layout, num_elements))
		return false;

	if (!pixel_shader_.Initialize(this->device_, shader_folder + L"PixelShader.cso"))
		return false;

	return true;
}

bool Graphics::InitializeScene()
{
	// Textured Square
	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, 1.0f, 0.0f, 1.0f), //Bottom Left   - [0]
		Vertex(-0.5f,  0.5f,  1.0f, 0.0f, 0.0f), //Top Left      - [1]
		Vertex(0.5f,   0.5f,  1.0f, 1.0f, 0.0f), //Top Right     - [2]
		Vertex(0.5f,   -0.5f, 1.0f, 1.0f, 1.0f), //Bottom Right  - [3]
	};

	// Load Vertex Data
	HRESULT hr = this->vertex_buffer_.Initialize(this->device_.Get(), v, ARRAYSIZE(v));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer");
		return false;
	}

	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	hr = this->indices_buffer_.Initialize(this->device_.Get(), indices, ARRAYSIZE(indices));
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create indices buffer.");
		return false;
	}

	//Load texture
	hr = DirectX::CreateWICTextureFromFile(this->device_.Get(), L"Data/Textures/piano.png", nullptr, texture_.GetAddressOf());
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create WIC texture from file.");
		return false;
	}

	// Initialize constant buffer(s)
	hr = this->constant_buffer_.Initialize(this->device_.Get(), this->device_context_.Get());
	if(FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
		return false;
	}

	return true;
}
