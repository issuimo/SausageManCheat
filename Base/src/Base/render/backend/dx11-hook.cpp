#include <pch.h>
#include "dx11-hook.h"

#include "Base/SEH.h"
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")

// D3X HOOK DEFINITIONS
using IDXGISwapChainPresent = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

// Definition of WndProc Hook. Its here to avoid dragging dependencies on <windows.h> types.
extern auto ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
static IDXGISwapChainPresent fnIDXGISwapChainPresent;

static auto __stdcall Present_Hook(IDXGISwapChain* pChain, const UINT SyncInterval, const UINT Flags) -> HRESULT {
	static BOOL g_bInitialised = false;
	if (!g_bInitialised) {
		_set_se_translator(SEH_exception::TranslateSEHtoCE);
		auto result = pChain->GetDevice(__uuidof(backend::pDevice), reinterpret_cast<void**>(&backend::pDevice));

		if (SUCCEEDED(result)) {
			backend::pDevice->GetImmediateContext(&backend::pContext);

			DXGI_SWAP_CHAIN_DESC sd;
			pChain->GetDesc(&sd);

			backend::DX11Events::InitializeEvent(sd.OutputWindow, backend::pDevice, backend::pContext, pChain);
			backend::pSwapChain = pChain;
			g_bInitialised = true;
		}
	}

	// render function
	if (g_bInitialised) { backend::DX11Events::RenderEvent(backend::pContext); }

	return fnIDXGISwapChainPresent(pChain, SyncInterval, Flags);
}

static auto findDirect11Present() -> IDXGISwapChainPresent {
	WNDCLASSEX wc{0};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = TEXT("Class");

	if (!RegisterClassEx(&wc)) { return nullptr; }

	HWND hWnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 0, 0, NULL, NULL, NULL, nullptr);

	IDXGISwapChain* pSwapChain;

	D3D_FEATURE_LEVEL featureLevel;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE; //((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Main D3D11 Objects
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11Device* pDevice = nullptr;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, nullptr, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext)) && FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext))) {
		DestroyWindow(swapChainDesc.OutputWindow);
		UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

		return nullptr;
	}

	DWORD_PTR* pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChain);
	pSwapChainVtable = reinterpret_cast<DWORD_PTR*>(pSwapChainVtable[0]);

	DWORD dwOld;
	auto oPresent = reinterpret_cast<IDXGISwapChainPresent>(pSwapChainVtable[8]);
	VirtualProtect(pSwapChainVtable, 0x1000, PAGE_EXECUTE_READWRITE, &dwOld);
	pSwapChainVtable[8] = reinterpret_cast<DWORD_PTR>(Present_Hook);
	VirtualProtect(pSwapChainVtable, 0x1000, dwOld, &dwOld);

	pDevice->Release();
	//pContext->Release();
	pSwapChain->Release();

	DestroyWindow(swapChainDesc.OutputWindow);
	UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));

	return oPresent;
}

auto backend::InitializeDX11Hooks() -> void {
	LOG_DEBUG("初始化D3D11钩子: 已启动.");
	fnIDXGISwapChainPresent = findDirect11Present();
	if (fnIDXGISwapChainPresent == nullptr) {
		LOG_ERROR("找不到D3D11的 'Present' 函数.");
		return;
	}
	LOG_DEBUG("SwapChain Present: %p", fnIDXGISwapChainPresent);
	LOG_DEBUG("初始化D3D11钩子: 完成.");
}

auto backend::LoadTextureFromMemory(LPBYTE image_data, int image_width, int image_height, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height) -> bool {
	if (pDevice == nullptr) { return false; }

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = nullptr;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;

	return true;
}
