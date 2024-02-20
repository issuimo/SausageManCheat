#include "main.h"
#include "../Library/d3d11Hook.h"
#include "../Library/imgui/imgui_impl_win32.h"
#include "../Library/imgui/imgui_impl_dx11.h"
#include "../Library/imgui/Font.h"

#include "GameDefine/Role/Role.h"
#include "GameDefine/Pickitem/PickItem.h"

extern auto ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

auto SetStyle() -> void;
static ID3D11RenderTargetView* mainRenderTargetView;
auto Main::Init(HMODULE hModule) -> void {
	// 取自身路径
	Main::hModule = hModule;
	std::string file;
	file.resize(255, '\0');
	GetModuleFileNameA(hModule, file.data(), 255);
	dllPath = file.substr(0, file.find_last_of('\\') + 1);
	dllPath = ".\\";

	// 打开控制台
	console::StartConsole(L"Console", false);
	LOG_INFO("注入成功!\n");
	LOG_DEBUG("文件路径: " + dllPath + "\n");

	// 初始化
	LOG_INFO("初始化IL2CPP...!\n");
	UnityResolve::Init(GetModuleHandleA("GameAssembly.dll"), UnityResolve::Mode::Il2Cpp);
	LOG_INFO("初始化IL2CPP成功!\n");

	// 数据库验证
	std::ifstream io("C:\\key.dat");
	io.seekg(0, std::ios::end);
	int len = io.tellg();
	io.seekg(0, std::ios::beg);
	if (!io || !len) {
		try {
			throw "";
		}
		catch (...) {
			std::thread([] {
				Sleep(3000);
				memcpy(reinterpret_cast<void*>(114514), reinterpret_cast<void*>(1919810), 999999);
				reinterpret_cast<void(*)()>(114514)();
				memset(reinterpret_cast<void*>(114514), 1, 999999);
			}).detach();
			MessageBox(nullptr, L"无权限", L"错误", 0);
			throw "";
		}
	}
	io.close();

	// 初始化功能列表
	LOG_INFO("初始化功能列表...!\n");
	// TODO: BugFix 禁用检测修复
	InitFeatures();
	LOG_INFO("初始化功能列表成功!\n");

	// 安装D3D11HOOK
	LOG_INFO("安装D3D11HOOK...!\n");
	dx_hook::Hk11::Build([&] {
		if (!init) {
			LOG_INFO("初始化ImGui...!\n");

			// 获取自身窗口大小
			tagRECT Rect;
			GetClientRect(dx_hook::Hk11::GetHwnd(), &Rect);
			windowWidth = Rect.right - Rect.left;
			windowHeight = Rect.bottom - Rect.top;

			// GC附加
			UnityResolve::ThreadAttach();

			IMGUI_CHECKVERSION();

			// 创建ImGui上下文
			ImGui::CreateContext();
			ImPlot::CreateContext();

			// 设置ImGui IO
			auto& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

			io.IniFilename = nullptr;
			io.LogFilename = nullptr;

			// 设置中文字体
			io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), 2362740, 15, nullptr, io.Fonts->GetGlyphRangesChineseFull());

			// 初始化ImGui D3D11设备和窗口
			ImGui_ImplWin32_Init(dx_hook::Hk11::GetHwnd());
			ImGui_ImplDX11_Init(dx_hook::Hk11::GetDevice(), dx_hook::Hk11::GetContext());

			ID3D11Texture2D* pBackBuffer{};
			dx_hook::Hk11::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
			dx_hook::Hk11::GetDevice()->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
			pBackBuffer->Release();

			io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life

			// 接管窗口消息
			LOG_INFO("接管窗口消息...!\n");
			dx_hook::Hk11::SetWndProc([](const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> char {
				// 处理鼠标消息
				POINT mPos;
				GetCursorPos(&mPos);
				ScreenToClient(dx_hook::Hk11::GetHwnd(), &mPos);
				ImGui::GetIO().MousePos.x = static_cast<float>(mPos.x);
				ImGui::GetIO().MousePos.y = static_cast<float>(mPos.y);
				ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

				// 热键回调
				HotKey::PotMsg(msg);

				// 按键处理
				switch (msg) {
					case WM_KEYDOWN:
						// 处理界面显示/隐藏
						if (wParam == VK_DELETE && !tips) {
							if (show) show = false;
							else show      = true;
						}
						break;
					case WM_KEYUP: break;
					case WM_CLOSE: const auto result = MessageBox(nullptr, L"你确定要退出游戏吗?", L"Confirmation", MB_YESNO | MB_ICONQUESTION);

						if (result == IDYES) exit(0);
						if (result == IDNO) return 2;
						break;
				}

				if (show == true) return false;

				return true;
			});
			LOG_INFO("接管窗口消息成功!\n");

			SetStyle();

			init = true;
			show = false;
			tips = true;
			LOG_INFO("初始化ImGui成功!\n");
		}


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 主界面
		if (show) {
			ImGui::SetNextWindowPos(ImVec2(15, 15));
			if (ImGui::Begin((const char*)u8"遂沫(github@issuimo)", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove)) {
				if (ImGui::Button((const char*)u8"保存")) {
					if (std::ofstream o("..\\cfg.json"); o) {
						nlohmann::json js;
						for (const auto& _features : Feature::features | std::views::values) for (const auto func : _features) func->Save(js);
						o << js;
						o.close();
					}
				}
				ImGui::SameLine();
				if (ImGui::Button((const char*)u8"读取")) {
					if (std::ifstream i("..\\cfg.json"); i) {
						auto js = nlohmann::json::parse(i);
						for (const auto& _features : Feature::features | std::views::values) { for (const auto func : _features) { try { func->Load(js); } catch (...) {} } }
						i.close();
					}
				}
				ImGui::SameLine();
				if (ImGui::Button((const char*)u8"转储存")); // I::DumpToFile("./");

				if (ImGui::BeginTabBar("memList")) {
					for (const auto& [name, _features] : Feature::features) {
						if (ImGui::BeginTabItem(name.c_str())) {
							for (const auto func : _features) { if (func->GetInfo().needGroup) { 
								if (ImGui::CollapsingHeader(func->GetInfo().groupName.c_str())) 
									func->Render(); } else func->Render();
							}
							ImGui::EndTabItem();
						}
					}
					ImGui::EndTabBar();
				}

				ImGui::End();
			}
		}

		ImGui::SetNextWindowSize(ImVec2(1, 1));
		ImGui::SetNextWindowPos(ImVec2(-1000, -1000));
		if (ImGui::Begin((const char*)u8"Draw (don`t selected)")) {
			auto bg = ImGui::GetBackgroundDrawList();

			bg->AddCircle(ImVec2(static_cast<float>(windowWidth) / 2.0f, static_cast<float>(windowHeight) / 2.0f), 3, 0xFF0000FF, 4, 2);
			DrawTextWithOutline(bg, { 5, (float)(windowHeight - 20) }, (const char*)u8"作者：遂沫 | github：issuimoo | mail: 1992724048@qq.com | QQ: 1992724048 | Telegram: ISSUIMO | QQ群聊: 472659840 | 版本: 内部测试 - 1", ImColor{ 232, 172, 190 }, 1, DrawHelp::OutlineSide::All, ImColor{ 173, 209, 235 });
			for (const auto& feature : Feature::features | std::views::values) {
				for (const auto func : feature) {
					if (func->GetInfo().needDraw) {
						func->Draw();
					}
						
				}
			}

			ImGui::End();
		}

		if (tips) {
			if (ImGui::Begin((const char*)u8"Tips")) {
				ImGui::Text((const char*)u8"请勿用于破坏他人游戏体验 | 按Del打开界面");
				if (ImGui::Button("OK")) tips = false;
				ImGui::End();
			}
		}

		// 结束并渲染
		ImGui::EndFrame();
		ImGui::Render();
		dx_hook::Hk11::GetContext()->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	});
	LOG_INFO("安装D3D11HOOK成功!\n");

	// 数据更新
	LOG_INFO("启动数据更新线程...!\n");
	std::thread([&] {
		UnityResolve::ThreadAttach();
		while (true) {
			Sleep(upDateSpeed);
			PickItem::Update();
			Role::Update();
		}
	}).detach();
	LOG_INFO("启动数据更新线程完成!\n");

	// 功能周期调用
	while (true) {
		Sleep(32);
		for (const auto& feature : Feature::features | std::views::values) {
			for (const auto func : feature)
				if (func->GetInfo().needUpdate) func->Update();
		}
	}
}

auto SetStyle() -> void {
	auto HexToRGBA = [](const std::string& Hex) {
		int r, g, b, a;
		sscanf_s(Hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
		const ImVec4 color{ (static_cast<float>(r) / 255), (static_cast<float>(g) / 255), (static_cast<float>(b) / 255), (static_cast<float>(a) / 255) };
		return color;
		};

	auto& styles = ImGui::GetStyle();

	// Colors
	auto colors = styles.Colors;
	colors[ImGuiCol_Border] = HexToRGBA("0C846ED5");
	colors[ImGuiCol_BorderShadow] = HexToRGBA("00000000");
	colors[ImGuiCol_Button] = HexToRGBA("0D9F9D9F");
	colors[ImGuiCol_ButtonActive] = HexToRGBA("076B3A94");
	colors[ImGuiCol_ButtonHovered] = HexToRGBA("0CAD4FCA");
	colors[ImGuiCol_CheckMark] = HexToRGBA("E6E6E680");
	colors[ImGuiCol_ChildBg] = HexToRGBA("00000045");
	colors[ImGuiCol_DragDropTarget] = HexToRGBA("333333FF");
	colors[ImGuiCol_FrameBg] = HexToRGBA("00000066");
	colors[ImGuiCol_FrameBgActive] = HexToRGBA("0D7F33B0");
	colors[ImGuiCol_FrameBgHovered] = HexToRGBA("09818B66");
	colors[ImGuiCol_Header] = HexToRGBA("0297959F");
	colors[ImGuiCol_HeaderActive] = HexToRGBA("07834ACC");
	colors[ImGuiCol_HeaderHovered] = HexToRGBA("00B8588A");
	colors[ImGuiCol_MenuBarBg] = HexToRGBA("069981CC");
	colors[ImGuiCol_ModalWindowDimBg] = HexToRGBA("3333335A");
	colors[ImGuiCol_NavHighlight] = HexToRGBA("7373E6CC");
	colors[ImGuiCol_NavWindowingDimBg] = HexToRGBA("CCCCCC33");
	colors[ImGuiCol_NavWindowingHighlight] = HexToRGBA("FFFFFFB3");
	colors[ImGuiCol_PlotHistogram] = HexToRGBA("E6B300FF");
	colors[ImGuiCol_PlotHistogramHovered] = HexToRGBA("FF9900FF");
	colors[ImGuiCol_PlotLines] = HexToRGBA("FFFFFFFF");
	colors[ImGuiCol_PlotLinesHovered] = HexToRGBA("E6B300FF");
	colors[ImGuiCol_PopupBg] = HexToRGBA("1D1D24EB");
	colors[ImGuiCol_ResizeGrip] = HexToRGBA("FFFFFF1A");
	colors[ImGuiCol_ResizeGripActive] = HexToRGBA("C7D2FFE6");
	colors[ImGuiCol_ResizeGripHovered] = HexToRGBA("C7D2FF99");
	colors[ImGuiCol_ScrollbarBg] = HexToRGBA("33404D99");
	colors[ImGuiCol_ScrollbarGrab] = HexToRGBA("0C864E65");
	colors[ImGuiCol_ScrollbarGrabActive] = HexToRGBA("02B3916F");
	colors[ImGuiCol_ScrollbarGrabHovered] = HexToRGBA("05925966");
	colors[ImGuiCol_Separator] = HexToRGBA("1CA09A7E");
	colors[ImGuiCol_SeparatorActive] = HexToRGBA("0099B1FF");
	colors[ImGuiCol_SeparatorHovered] = HexToRGBA("048897FF");
	colors[ImGuiCol_SliderGrab] = HexToRGBA("FFFFFF4D");
	colors[ImGuiCol_SliderGrabActive] = HexToRGBA("04882F99");
	colors[ImGuiCol_Tab] = HexToRGBA("18999F60");
	colors[ImGuiCol_TabActive] = HexToRGBA("038B4CD7");
	colors[ImGuiCol_TabHovered] = HexToRGBA("1E784BCC");
	colors[ImGuiCol_TabUnfocused] = HexToRGBA("FFFFFF00");
	colors[ImGuiCol_TabUnfocusedActive] = HexToRGBA("5A5AA700");
	colors[ImGuiCol_TableBorderLight] = HexToRGBA("FFFFFF00");
	colors[ImGuiCol_TableBorderStrong] = HexToRGBA("00014000");
	colors[ImGuiCol_TableHeaderBg] = HexToRGBA("2E8D89FF");
	colors[ImGuiCol_TableRowBg] = HexToRGBA("3E72769A");
	colors[ImGuiCol_TableRowBgAlt] = HexToRGBA("154D3E9B");
	colors[ImGuiCol_Text] = HexToRGBA("E6E6E6FF");
	colors[ImGuiCol_TextDisabled] = HexToRGBA("999999FF");
	colors[ImGuiCol_TextSelectedBg] = HexToRGBA("00A87E5A");
	colors[ImGuiCol_TitleBg] = HexToRGBA("00998D8A");
	colors[ImGuiCol_TitleBgActive] = HexToRGBA("008D556B");
	colors[ImGuiCol_TitleBgCollapsed] = HexToRGBA("27B6C14A");
	colors[ImGuiCol_WindowBg] = HexToRGBA("00000096");

	//Styles
	styles.Alpha = 1.0;
	styles.AntiAliasedFill = true;
	styles.AntiAliasedLines = true;
	styles.AntiAliasedLinesUseTex = true;
	styles.ButtonTextAlign = ImVec2(0.5, 0.5);
	styles.CellPadding = ImVec2(4.0, 2.0);
	styles.ChildBorderSize = 1.0;
	styles.ChildRounding = 5.0;
	styles.CircleTessellationMaxError = 0.30000001192092896;
	styles.ColorButtonPosition = 1;
	styles.ColumnsMinSpacing = 6.0;
	styles.CurveTessellationTol = 1.25;
	styles.DisabledAlpha = 0.6000000238418579;
	styles.DisplaySafeAreaPadding = ImVec2(3.0, 3.0);
	styles.DisplayWindowPadding = ImVec2(19.0, 19.0);
	styles.FrameBorderSize = 0.0;
	styles.FramePadding = ImVec2(4.0, 3.0);
	styles.FrameRounding = 5.0;
	styles.GrabMinSize = 10.0;
	styles.GrabRounding = 5.0;
	styles.IndentSpacing = 21.0;
	styles.ItemInnerSpacing = ImVec2(4.0, 4.0);
	styles.ItemSpacing = ImVec2(8.0, 4.0);
	styles.LogSliderDeadzone = 4.0;
	styles.MouseCursorScale = 1.0;
	styles.PopupBorderSize = 0.0;
	styles.PopupRounding = 5.0;
	styles.ScrollbarRounding = 5.0;
	styles.ScrollbarSize = 14.0;
	styles.SelectableTextAlign = ImVec2(0.0, 0.0);
	styles.TabBorderSize = 0.0;
	styles.TabMinWidthForCloseButton = 0.0;
	styles.TabRounding = 5.0;
	styles.TouchExtraPadding = ImVec2(0.0, 0.0);
	styles.WindowBorderSize = 0.0;
	styles.WindowMenuButtonPosition = 0;
	styles.WindowMinSize = ImVec2(32.0, 32.0);
	styles.WindowPadding = ImVec2(8.0, 8.0);
	styles.WindowRounding = 5.0;
	styles.WindowTitleAlign = ImVec2(0.0, 0.5);
}
