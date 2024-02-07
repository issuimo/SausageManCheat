#pragma once

#ifndef _DX11HOOK_
#define _DX11HOOK_

#include <d3d11.h>
#include <functional>

#pragma comment(lib, "d3d11.lib")

namespace dx_hook {
	class Hk11 {
		using IDXGISwapChainPresent = HRESULT(WINAPI*)(IDXGISwapChain*, UINT, UINT);

	public:
		static auto Build(const std::function<void()>& fn) -> bool {
			if (!fn) return false;
			present = fn;

			WNDCLASSEX windowClass;
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = DefWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle(nullptr);
			windowClass.hIcon = nullptr;
			windowClass.hCursor = nullptr;
			windowClass.hbrBackground = nullptr;
			windowClass.lpszMenuName = nullptr;
			windowClass.lpszClassName = L"IME";
			windowClass.hIconSm = nullptr;

			::RegisterClassEx(&windowClass);

			const HWND window = ::CreateWindow(windowClass.lpszClassName, L"IME", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

			HMODULE libD3D11;
			if ((libD3D11 = ::GetModuleHandle(L"d3d11.dll")) == nullptr) {
				DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return false;
			}

			void* D3D11CreateDeviceAndSwapChain;
			if ((D3D11CreateDeviceAndSwapChain = GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain")) == nullptr) {
				DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return false;
			}

			D3D_FEATURE_LEVEL           featureLevel;
			constexpr D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

			DXGI_RATIONAL refreshRate;
			refreshRate.Numerator = 60;
			refreshRate.Denominator = 1;

			DXGI_MODE_DESC bufferDesc;
			bufferDesc.Width = 100;
			bufferDesc.Height = 100;
			bufferDesc.RefreshRate = refreshRate;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			DXGI_SAMPLE_DESC sampleDesc;
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;

			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc = sampleDesc;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = window;
			swapChainDesc.Windowed = 1;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			IDXGISwapChain* swapChain;
			ID3D11Device* device;
			ID3D11DeviceContext* context;

			if (static_cast<long(__stdcall*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**)>(D3D11CreateDeviceAndSwapChain)(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context) < 0) {
				DestroyWindow(window);
				::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
				return false;
			}

			const auto pVMT = reinterpret_cast<uintptr_t*>(swapChain);
			const auto VMT  = reinterpret_cast<uintptr_t*>(pVMT[0]);
			vmt = VMT;
			DWORD      dwOld;
			oPresent = reinterpret_cast<IDXGISwapChainPresent>(VMT[8]);
			VirtualProtect(VMT, 0x1000, PAGE_EXECUTE_READWRITE, &dwOld);
			VMT[8] = reinterpret_cast<DWORD_PTR>(MyPresent);
			VirtualProtect(VMT, 0x1000, dwOld, &dwOld);
			device->Release();
			context->Release();
			swapChain->Release();
			DestroyWindow(window);
			return false;
		}

		static auto SetWndProc(char (*wndProc)(HWND, UINT, WPARAM, LPARAM)) -> void {
			Hk11::wndProc    = wndProc;
			oldWndProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(NewWndProc));
		}

		static auto Unbuild() -> void {
			DWORD      dwOld;
			VirtualProtect(vmt, 0x1000, PAGE_EXECUTE_READWRITE, &dwOld);
			vmt[8] = reinterpret_cast<DWORD_PTR>(oPresent);
			VirtualProtect(vmt, 0x1000, dwOld, &dwOld);
		}

		static auto GetHwnd() -> HWND { return hWnd; }
		static auto GetDevice() -> ID3D11Device* { return gDevice; }
		static auto GetSwapChain() -> IDXGISwapChain* { return gSwapChain; }
		static auto GetContext() -> ID3D11DeviceContext* { return gContext; }
		static auto GetTargetView() -> ID3D11RenderTargetView* const * { return &gTargetView; }

	private:
		inline static bool init{false};

		inline static HWND    hWnd{};
		inline static WNDPROC oldWndProc{};

		inline static std::function<void()> present;
		inline static char (*               wndProc)(HWND, UINT, WPARAM, LPARAM);

		inline static ID3D11Device*           gDevice{};
		inline static IDXGISwapChain*         gSwapChain{};
		inline static ID3D11DeviceContext*    gContext{};
		inline static ID3D11RenderTargetView* gTargetView{};
		inline static IDXGISwapChainPresent   oPresent{};
		inline static uintptr_t*              vmt{};

		static auto WINAPI MyPresent(IDXGISwapChain* a, const UINT b, const UINT c) -> HRESULT {
			if (!init) {
				gSwapChain        = a;
				if (const auto result = a->GetDevice(__uuidof(gDevice), reinterpret_cast<void**>(&gDevice)); SUCCEEDED(result)) {
					gDevice->GetImmediateContext(&gContext);
					DXGI_SWAP_CHAIN_DESC sd;
					a->GetDesc(&sd);
					hWnd = sd.OutputWindow;
					ID3D11Texture2D* pBackBuffer{};
					a->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
					gDevice->CreateRenderTargetView(pBackBuffer, nullptr, &gTargetView);
					pBackBuffer->Release();
					init = true;
				}
			}

			present();

			return oPresent(a, b, c);
		}

		static auto NewWndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
			const auto ret = wndProc(hWnd, uMsg, wParam, lParam);
			if (ret == 1) return CallWindowProc(oldWndProc, hWnd, uMsg, wParam, lParam);
			if (ret == 0) return DefWindowProc(hWnd, uMsg, wParam, lParam);
			if (ret == 2) return NULL;
			return NULL;
		}
	};
}

#endif
