#include <pch.h>
#include "renderer.h"

#include "Base/SEH.h"

#include <backends/imgui_impl_dx11.h>
#pragma comment(lib, "dxgi")

#include <backends/imgui_impl_win32.h>

#include <Base/util.h>
#include <Base/render/backend/dx11-hook.h>

#include <Base/ResourceLoader.h>
#include <Base/cheat/misc/Settings.h>

#include "Base/UnityResolve.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace renderer
{
	struct Data
	{
		LPBYTE data;
		DWORD size;
	};

	static std::unordered_set<void*> _inputLockers;

	static float _globalFontSize = 16.0f;
	static bool _isCustomFontLoaded = false;

	static constexpr int _fontSizeStep = 1;
	static constexpr int _fontSizeMax = 64;
	static constexpr int _fontsCount = _fontSizeMax / _fontSizeStep;
	static std::array<ImFont*, _fontsCount> _fonts;

	static Data _customFontData{};

	static WNDPROC OriginalWndProcHandler;
	static ID3D11RenderTargetView* mainRenderTargetView;

	static void OnRenderDX11(ID3D11DeviceContext* pContext);
	static void OnInitializeDX11(HWND window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pChain);

	int windowHeight;
	int windowWidth;
	HWND hwnd;

	void Init(LPBYTE fontData, DWORD fontDataSize, DXVersion version)
	{
		_customFontData = { fontData, fontDataSize };

		LOG_DEBUG("初始化IMGui...");

		switch (version)
		{
		case DXVersion::D3D11:
			backend::DX11Events::RenderEvent += FUNCTION_HANDLER(OnRenderDX11);
			backend::DX11Events::InitializeEvent += FUNCTION_HANDLER(OnInitializeDX11);
			backend::InitializeDX11Hooks();
			break;
		case DXVersion::D3D12:
		case DXVersion::D3D9:
		case DXVersion::D3D10:
		default:
			LOG_ERROR("使用不支持的DX版本.");
		}
	}

	void SetInputLock(void* id, bool value)
	{
		if (value)
			AddInputLocker(id);
		else
			RemoveInputLocker(id);
	}

	void AddInputLocker(void* id)
	{
		if (!_inputLockers.contains(id))
			_inputLockers.insert(id);
	}

	void RemoveInputLocker(void* id)
	{
		if (_inputLockers.contains(id))
			_inputLockers.erase(id);
	}

	bool IsInputLocked()
	{
		return !_inputLockers.empty();
	}

	ImFont* GetFontBySize(float fontSize)
	{
		if (!_isCustomFontLoaded)
		{
			ImGuiIO& io = ImGui::GetIO();
			return io.FontDefault;
		}
		int fontSizeInt = static_cast<int>(fontSize);
		int fontIndex = fontSizeInt / _fontSizeStep +
			(fontSizeInt % _fontSizeStep > (_fontSizeStep / 2) ? 1 : 0) - 1;
		fontIndex = std::clamp(fontIndex, 0, _fontsCount - 1);
		return _fonts[fontIndex];
	}

	float GetScaleByFontSize(float fontSize)
	{
		if (!_isCustomFontLoaded)
		{
			ImGuiIO& io = ImGui::GetIO();
			return fontSize / io.FontDefault->FontSize;
		}

		int fontSizeInt = static_cast<int>(fontSize);
		int fontIndex = fontSizeInt / _fontSizeStep;
		int fontAligned = fontIndex * _fontSizeStep +
			((fontSizeInt % _fontSizeStep) > _fontSizeStep / 2 ? _fontSizeStep : 0);
		fontAligned = std::clamp(fontAligned, _fontSizeStep, _fontSizeMax);

		return fontSize / static_cast<float>(fontAligned);
	}

	void SetGlobalFontSize(float globalFontSize)
	{
		_globalFontSize = globalFontSize;
	}

	float GetGlobalFontSize()
	{
		return _globalFontSize;
	}

	static void LoadCustomFont()
	{
		if (_customFontData.data == nullptr)
			return;
		const ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromMemoryTTF(_customFontData.data, _customFontData.size, 16, nullptr, io.Fonts->GetGlyphRangesChineseFull());
		/*for (int i = 0; i < _fontsCount; i++) 原方案,这个有问题，加载中文字体会闪退
		{
			const ImGuiIO& io = ImGui::GetIO();
			const auto newFont = 
			if (newFont == nullptr) {
				return;
			}

			_fonts[i] = newFont;
		}*/
		_isCustomFontLoaded = true;
	}

	static void LoadImGuiStyles()
	{
		auto themes = cheat::feature::Settings::GetInstance();
		themes->Init();
	}

	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static void OnInitializeDX11(HWND window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, IDXGISwapChain* pChain)
	{
		UnityResolve::ThreadAttach();
		LOG_DEBUG("ImGUI: DirectX11后端初始化成功.");

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		static const std::string imguiPath = (util::GetCurrentPath() / "imgui.ini").string();
		io.IniFilename = imguiPath.c_str();

		LoadCustomFont();
		LoadImGuiStyles();

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		OriginalWndProcHandler = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(hWndProc)));

		WINDOWINFO windowInfo = { sizeof(WINDOWINFO) };
		GetWindowInfo(window, &windowInfo);
		windowWidth = windowInfo.rcClient.right - windowInfo.rcClient.left;
		windowHeight = windowInfo.rcClient.bottom - windowInfo.rcClient.top;
		hwnd = window;
		LOG_DEBUG("窗口句柄: %p", window);
		LOG_DEBUG("窗口大小: %d x %d", windowWidth, windowHeight);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);

		ID3D11Texture2D* pBackBuffer;
		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
		pBackBuffer->Release();

		io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life
	}

	static void OnRenderDX11(ID3D11DeviceContext* pContext)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.FontDefault = GetFontBySize(_globalFontSize);
		ImGui::NewFrame();

		events::RenderEvent();

		ImGui::EndFrame();
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	static LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static bool init = false;
		if (!init) {
			_set_se_translator(SEH_exception::TranslateSEHtoCE);
			UnityResolve::ThreadAttach();
			init = true;
		}

		POINT mPos;
		GetCursorPos(&mPos);
		ScreenToClient(hWnd, &mPos);
		ImGui::GetIO().MousePos.x = static_cast<float>(mPos.x);
		ImGui::GetIO().MousePos.y = static_cast<float>(mPos.y);

		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		if (!events::WndProcEvent(hWnd, uMsg, wParam, lParam))
			return true;

		short key;
		int code;
		bool keyUpEvent = true;
		switch (uMsg)
		{
		case WM_LBUTTONUP:
			key = VK_LBUTTON;
			break;
		case WM_RBUTTONUP:
			key = VK_RBUTTON;
			break;
		case WM_MBUTTONUP:
			key = VK_MBUTTON;
			break;
		case WM_XBUTTONUP:
			key = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
			break;
		case WM_KEYUP:
			key = static_cast<short>(wParam);
			break;
		case WM_CLOSE: if (MessageBoxW(nullptr, L"确定要退出吗?", L"提示", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				TerminateProcess(GetCurrentProcess(), 0);
			}
		default:
			keyUpEvent = false;
			break;
		}

		bool canceled = false;
		if (keyUpEvent)
			canceled = !events::KeyUpEvent(key);

		if (IsInputLocked() || canceled)
			return true;

		return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
	}
}