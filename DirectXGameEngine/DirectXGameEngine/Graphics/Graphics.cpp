#include "Graphics.h"

bool Graphics::Initialize(const HWND hwnd, const int width, const int height)
{
	if (!InitializeDirectX(hwnd, width, height))
		return false;

	if (!InitializeShaders())
		return false;

	return true;
}

void Graphics::RenderFrame() const
{
	constexpr float bg_color[] = {1.f, 0.f, 0.f, 1.0f};
	this->device_context_->ClearRenderTargetView(this->render_target_view_.Get(), bg_color);
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

	this->device_context_->OMSetRenderTargets(1, this->render_target_view_.GetAddressOf(), NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	// Set the Viewport
	this->device_context_->RSSetViewports(1, &viewport);

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
			DXGI_FORMAT_R32G32_FLOAT, // R32-G32-B32-A32
			0,
			0,
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
