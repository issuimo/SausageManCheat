#include <pch.h>
#include "Settings.h"

#include <Base/cheat/CheatManagerBase.h>
#include <Base/render/renderer.h>
#include <Base/render/gui-util.h>
#include <misc/cpp/imgui_stdlib.h>
#include <Base/util.h>

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

namespace cheat::feature
{
	Settings::Settings() :
		NF(f_MenuKey, "显示作弊菜单热键", "General", Hotkey(VK_DELETE)),
		NF(f_HotkeysEnabled, "菜单热键", "General", true),
		
		NF(f_StatusMove, "显示状态窗口", "General::StatusWindow", true),
		NF(f_StatusShow, "移动状态窗口", "General::StatusWindow", true),

		NF(f_InfoMove, "显示信息窗口", "General::InfoWindow", true),
		NF(f_InfoShow, "移动信息窗口", "General::InfoWindow", true),

		NF(f_FpsMove, "移动FPS指示器", "General::FPS", false),
		NF(f_FpsShow, "显示FPS指示器", "General::FPS", true),

		NF(f_NotificationsShow, "显示通知", "General::Notify", true),
		NF(f_NotificationsDelay, "通知显示时长", "General::Notify", 500),

		NF(f_FileLogging, "文件日志记录", "General::Logging", false),
		NF(f_ConsoleLogging, "控制台日志记录", "General::Logging", true),

		NF(f_FastExitEnable, "快速退出", "General::FastExit", false),
		NF(f_HotkeyExit, "退出热键", "General::FastExit", Hotkey(VK_F12)),
		
		NF(f_FontSize, "字体大小", "General::Theme", 16.0f),
		NF(f_ShowStyleEditor, "显示主题自定义窗口", "General::Theme", false),
		NFS(f_DefaultTheme, "主题", "General::Theme", ""),
		themesDir(util::GetCurrentPath() / "themes")

	{
		renderer::SetGlobalFontSize(f_FontSize);
		f_HotkeyExit.value().PressedEvent += MY_METHOD_HANDLER(Settings::OnExitKeyPressed);
	}

	const FeatureGUIInfo& Settings::GetGUIInfo() const
	{
		static const FeatureGUIInfo info{ "", "设置", false };
		return info;
	}

	ImVec4 HexToColor(std::string hexString)
	{
		int r, g, b, a;
		sscanf_s(hexString.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
		ImVec4 color{ (float(r) / 255), (float(g) / 255), (float(b) / 255), (float(a) / 255) };
		return color;
	}

	std::string ColorToHex(ImVec4& color)
	{
		char hex[16];
		snprintf(hex, sizeof(hex), "%02x%02x%02x%02x",
			static_cast<int>(ceil(color.x * 255)),
			static_cast<int>(ceil(color.y * 255)),
			static_cast<int>(ceil(color.z * 255)),
			static_cast<int>(ceil(color.w * 255))
		);
		for (int i = 0; i < 16; i++)
			hex[i] = toupper(hex[i]);
		return hex;
	}

	static void DefaultTheme()
	{
		auto& styles = ImGui::GetStyle();

		// Colors
		const auto colors = styles.Colors;
		colors[ImGuiCol_Border] = HexToColor("0C846ED5");
		colors[ImGuiCol_BorderShadow] = HexToColor("00000000");
		colors[ImGuiCol_Button] = HexToColor("0D9F9D9F");
		colors[ImGuiCol_ButtonActive] = HexToColor("076B3A94");
		colors[ImGuiCol_ButtonHovered] = HexToColor("0CAD4FCA");
		colors[ImGuiCol_CheckMark] = HexToColor("E6E6E680");
		colors[ImGuiCol_ChildBg] = HexToColor("00000045");
		colors[ImGuiCol_DragDropTarget] = HexToColor("333333FF");
		colors[ImGuiCol_FrameBg] = HexToColor("00000066");
		colors[ImGuiCol_FrameBgActive] = HexToColor("0D7F33B0");
		colors[ImGuiCol_FrameBgHovered] = HexToColor("09818B66");
		colors[ImGuiCol_Header] = HexToColor("0297959F");
		colors[ImGuiCol_HeaderActive] = HexToColor("07834ACC");
		colors[ImGuiCol_HeaderHovered] = HexToColor("00B8588A");
		colors[ImGuiCol_MenuBarBg] = HexToColor("069981CC");
		colors[ImGuiCol_ModalWindowDimBg] = HexToColor("3333335A");
		colors[ImGuiCol_NavHighlight] = HexToColor("7373E6CC");
		colors[ImGuiCol_NavWindowingDimBg] = HexToColor("CCCCCC33");
		colors[ImGuiCol_NavWindowingHighlight] = HexToColor("FFFFFFB3");
		colors[ImGuiCol_PlotHistogram] = HexToColor("E6B300FF");
		colors[ImGuiCol_PlotHistogramHovered] = HexToColor("FF9900FF");
		colors[ImGuiCol_PlotLines] = HexToColor("FFFFFFFF");
		colors[ImGuiCol_PlotLinesHovered] = HexToColor("E6B300FF");
		colors[ImGuiCol_PopupBg] = HexToColor("1D1D24EB");
		colors[ImGuiCol_ResizeGrip] = HexToColor("FFFFFF1A");
		colors[ImGuiCol_ResizeGripActive] = HexToColor("C7D2FFE6");
		colors[ImGuiCol_ResizeGripHovered] = HexToColor("C7D2FF99");
		colors[ImGuiCol_ScrollbarBg] = HexToColor("33404D99");
		colors[ImGuiCol_ScrollbarGrab] = HexToColor("0C864E65");
		colors[ImGuiCol_ScrollbarGrabActive] = HexToColor("02B3916F");
		colors[ImGuiCol_ScrollbarGrabHovered] = HexToColor("05925966");
		colors[ImGuiCol_Separator] = HexToColor("1CA09A7E");
		colors[ImGuiCol_SeparatorActive] = HexToColor("0099B1FF");
		colors[ImGuiCol_SeparatorHovered] = HexToColor("048897FF");
		colors[ImGuiCol_SliderGrab] = HexToColor("FFFFFF4D");
		colors[ImGuiCol_SliderGrabActive] = HexToColor("04882F99");
		colors[ImGuiCol_Tab] = HexToColor("18999F60");
		colors[ImGuiCol_TabActive] = HexToColor("038B4CD7");
		colors[ImGuiCol_TabHovered] = HexToColor("1E784BCC");
		colors[ImGuiCol_TabUnfocused] = HexToColor("FFFFFF00");
		colors[ImGuiCol_TabUnfocusedActive] = HexToColor("5A5AA700");
		colors[ImGuiCol_TableBorderLight] = HexToColor("FFFFFF00");
		colors[ImGuiCol_TableBorderStrong] = HexToColor("00014000");
		colors[ImGuiCol_TableHeaderBg] = HexToColor("2E8D89FF");
		colors[ImGuiCol_TableRowBg] = HexToColor("3E72769A");
		colors[ImGuiCol_TableRowBgAlt] = HexToColor("154D3E9B");
		colors[ImGuiCol_Text] = HexToColor("E6E6E6FF");
		colors[ImGuiCol_TextDisabled] = HexToColor("999999FF");
		colors[ImGuiCol_TextSelectedBg] = HexToColor("00A87E5A");
		colors[ImGuiCol_TitleBg] = HexToColor("00998D8A");
		colors[ImGuiCol_TitleBgActive] = HexToColor("008D556B");
		colors[ImGuiCol_TitleBgCollapsed] = HexToColor("27B6C14A");
		colors[ImGuiCol_WindowBg] = HexToColor("00000096");

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

	bool hasLoaded = false;
	void Settings::Init() {
		if (hasLoaded)
			return;
		
		if (!std::filesystem::exists(themesDir))
			std::filesystem::create_directory(themesDir);
		else
		{
			m_Themes.clear();
			for (const auto& file : std::filesystem::directory_iterator(themesDir))
				if (std::filesystem::path(file).extension() == ".json")
					ThemeImport(file);
		}

		if (m_Themes.count(f_DefaultTheme.value()) > 0)
			ApplyTheme(f_DefaultTheme.value());
		else
			DefaultTheme();

		hasLoaded = true;
	}

	bool IsThemeOldFormat(nlohmann::json data)
	{
		if (data.count("Colors") > 0 && data.count("Styles") > 0)
			return false;
		
		return true;
	}

	void Settings::ThemeImport(std::filesystem::directory_entry file)
	{
		nlohmann::json data;
		std::ifstream fs(file.path());
		fs >> data;

		Theme theme;
		auto themeName = std::filesystem::path(file).stem().string();
		bool isOldFormat = IsThemeOldFormat(data);

		if (isOldFormat)
		{
			for (auto& [colorName, colorValue] : data.items())
			{
				ImVec4 color{};
				color.x = colorValue[0].get<float>();
				color.y = colorValue[1].get<float>();
				color.z = colorValue[2].get<float>();
				color.w = colorValue[3].get<float>();

				theme.colors.insert({ colorName, color });
			}
		}
		else
		{
			for (auto& [colorName, colorValue] : data["Colors"].items())
				theme.colors.insert({ colorName, HexToColor(colorValue) });

			for (auto& [styleName, styleValue] : data["Styles"].items())
			{
				if(styleValue.is_array())
					theme.styles.insert({ styleName, ImVec2(styleValue.at(0), styleValue.at(1)) });
				else if (styleValue.is_number_integer())
					theme.styles.insert({ styleName, styleValue.get<int>() });
				else if(styleValue.is_boolean())
					theme.styles.insert({ styleName, styleValue.get<bool>() });
				else 
					theme.styles.insert({ styleName, styleValue.get<float>() });
			}
		}

		m_Themes.insert({ themeName,  theme });

		// Convert old format to new format
		if(isOldFormat)
			ThemeExport(themeName, true);
	}

	nlohmann::json GetCurrentStyles(ImGuiStyle& style)
	{
		nlohmann::json stylesData;
		stylesData["Alpha"] = style.Alpha;
		stylesData["DisabledAlpha"] = style.DisabledAlpha;
		stylesData["WindowPadding"] = { style.WindowPadding.x, style.WindowPadding.y };
		stylesData["WindowRounding"] = style.WindowRounding;
		stylesData["WindowBorderSize"] = style.WindowBorderSize;
		stylesData["WindowMinSize"] = { style.WindowMinSize.x, style.WindowMinSize.y };
		stylesData["WindowTitleAlign"] = { style.WindowTitleAlign.x, style.WindowTitleAlign.y };
		stylesData["WindowMenuButtonPosition"] = style.WindowMenuButtonPosition;
		stylesData["ChildRounding"] = style.ChildRounding;
		stylesData["ChildBorderSize"] = style.ChildBorderSize;
		stylesData["PopupRounding"] = style.PopupRounding;
		stylesData["PopupBorderSize"] = style.PopupBorderSize;
		stylesData["FramePadding"] = { style.FramePadding.x, style.FramePadding.y };
		stylesData["FrameRounding"] = style.FrameRounding;
		stylesData["FrameBorderSize"] = style.FrameBorderSize;
		stylesData["ItemSpacing"] = { style.ItemSpacing.x, style.ItemSpacing.y };
		stylesData["ItemInnerSpacing"] = { style.ItemInnerSpacing.x,style.ItemInnerSpacing.y };
		stylesData["CellPadding"] = { style.CellPadding.x, style.CellPadding.y };
		stylesData["TouchExtraPadding"] = { style.TouchExtraPadding.x, style.TouchExtraPadding.y };
		stylesData["IndentSpacing"] = style.IndentSpacing;
		stylesData["ColumnsMinSpacing"] = style.ColumnsMinSpacing;
		stylesData["ScrollbarSize"] = style.ScrollbarSize;
		stylesData["ScrollbarRounding"] = style.ScrollbarRounding;
		stylesData["GrabMinSize"] = style.GrabMinSize;
		stylesData["GrabRounding"] = style.GrabRounding;
		stylesData["LogSliderDeadzone"] = style.LogSliderDeadzone;
		stylesData["TabRounding"] = style.TabRounding;
		stylesData["TabBorderSize"] = style.TabBorderSize;
		stylesData["TabMinWidthForCloseButton"] = style.TabMinWidthForCloseButton;
		stylesData["ColorButtonPosition"] = style.ColorButtonPosition;
		stylesData["ButtonTextAlign"] = { style.ButtonTextAlign.x, style.ButtonTextAlign.y };
		stylesData["SelectableTextAlign"] = { style.SelectableTextAlign.x, style.SelectableTextAlign.y };
		stylesData["DisplayWindowPadding"] = { style.DisplayWindowPadding.x, style.DisplayWindowPadding.y };
		stylesData["DisplaySafeAreaPadding"] = { style.DisplaySafeAreaPadding.x, style.DisplaySafeAreaPadding.y };
		stylesData["MouseCursorScale"] = style.MouseCursorScale;
		stylesData["AntiAliasedLines"] = style.AntiAliasedLines;
		stylesData["AntiAliasedLinesUseTex"] = style.AntiAliasedLinesUseTex;
		stylesData["AntiAliasedFill"] = style.AntiAliasedFill;
		stylesData["CurveTessellationTol"] = style.CurveTessellationTol;
		stylesData["CircleTessellationMaxError"] = style.CircleTessellationMaxError;
		return stylesData;
	}

	void Settings::ThemeExport(std::string name, bool replace)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		nlohmann::json data;
		if (replace)
			for (auto& [colorName, colorValue] : m_Themes[name].colors)
				data["Colors"][colorName] = ColorToHex(colorValue);

		else {
			auto colors = style.Colors;
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				auto colorName = ImGui::GetStyleColorName(i);
				data["Colors"][colorName] = ColorToHex(colors[i]);
			}
		}

		data["Styles"] = GetCurrentStyles(style);

		std::ofstream file(themesDir / (name + ".json"));
		file << std::setw(4) << data << std::endl;
	}

	void Settings::ApplyTheme(std::string name)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		auto colors = style.Colors;
		auto& theme = m_Themes[name];

		// Applying Colors
		if (!theme.colors.empty()) {
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				auto& themeColor = theme.colors[ImGui::GetStyleColorName(i)];
				colors[i].x = themeColor.x;
				colors[i].y = themeColor.y;
				colors[i].z = themeColor.z;
				colors[i].w = themeColor.w;
			}
		}

		if (!theme.styles.empty())
		{
			// Applying Styles
			style.Alpha = std::any_cast<float>(theme.styles["Alpha"]);
			style.DisabledAlpha = std::any_cast<float>(theme.styles["DisabledAlpha"]);
			style.WindowPadding = std::any_cast<ImVec2>(theme.styles["WindowPadding"]);
			style.WindowRounding = std::any_cast<float>(theme.styles["WindowRounding"]);
			style.WindowBorderSize = std::any_cast<float>(theme.styles["WindowBorderSize"]);
			style.WindowMinSize = std::any_cast<ImVec2>(theme.styles["WindowMinSize"]);
			style.WindowTitleAlign = std::any_cast<ImVec2>(theme.styles["WindowTitleAlign"]);
			style.WindowMenuButtonPosition = ImGuiDir(std::any_cast<int>(theme.styles["WindowMenuButtonPosition"]));
			style.ChildRounding = std::any_cast<float>(theme.styles["ChildRounding"]);
			style.ChildBorderSize = std::any_cast<float>(theme.styles["ChildBorderSize"]);
			style.PopupRounding = std::any_cast<float>(theme.styles["PopupRounding"]);
			style.PopupBorderSize = std::any_cast<float>(theme.styles["PopupBorderSize"]);
			style.FramePadding = std::any_cast<ImVec2>(theme.styles["FramePadding"]);
			style.FrameRounding = std::any_cast<float>(theme.styles["FrameRounding"]);
			style.FrameBorderSize = std::any_cast<float>(theme.styles["FrameBorderSize"]);
			style.ItemSpacing = std::any_cast<ImVec2>(theme.styles["ItemSpacing"]);
			style.ItemInnerSpacing = std::any_cast<ImVec2>(theme.styles["ItemInnerSpacing"]);
			style.CellPadding = std::any_cast<ImVec2>(theme.styles["CellPadding"]);
			style.TouchExtraPadding = std::any_cast<ImVec2>(theme.styles["TouchExtraPadding"]);
			style.IndentSpacing = std::any_cast<float>(theme.styles["IndentSpacing"]);
			style.ColumnsMinSpacing = std::any_cast<float>(theme.styles["ColumnsMinSpacing"]);
			style.ScrollbarSize = std::any_cast<float>(theme.styles["ScrollbarSize"]);
			style.ScrollbarRounding = std::any_cast<float>(theme.styles["ScrollbarRounding"]);
			style.GrabMinSize = std::any_cast<float>(theme.styles["GrabMinSize"]);
			style.GrabRounding = std::any_cast<float>(theme.styles["GrabRounding"]);
			style.LogSliderDeadzone = std::any_cast<float>(theme.styles["LogSliderDeadzone"]);
			style.TabRounding = std::any_cast<float>(theme.styles["TabRounding"]);
			style.TabBorderSize = std::any_cast<float>(theme.styles["TabBorderSize"]);
			style.TabMinWidthForCloseButton = std::any_cast<float>(theme.styles["TabMinWidthForCloseButton"]);
			style.ColorButtonPosition = ImGuiDir(std::any_cast<int>(theme.styles["ColorButtonPosition"]));
			style.ButtonTextAlign = std::any_cast<ImVec2>(theme.styles["ButtonTextAlign"]);
			style.SelectableTextAlign = std::any_cast<ImVec2>(theme.styles["SelectableTextAlign"]);
			style.DisplayWindowPadding = std::any_cast<ImVec2>(theme.styles["DisplayWindowPadding"]);
			style.DisplaySafeAreaPadding = std::any_cast<ImVec2>(theme.styles["DisplaySafeAreaPadding"]);
			style.MouseCursorScale = std::any_cast<float>(theme.styles["MouseCursorScale"]);
			style.AntiAliasedLines = std::any_cast<bool>(theme.styles["AntiAliasedLines"]);
			style.AntiAliasedLinesUseTex = std::any_cast<bool>(theme.styles["AntiAliasedLinesUseTex"]);
			style.AntiAliasedFill = std::any_cast<bool>(theme.styles["AntiAliasedFill"]);
			style.CurveTessellationTol = std::any_cast<float>(theme.styles["CurveTessellationTol"]);
			style.CircleTessellationMaxError = std::any_cast<float>(theme.styles["CircleTessellationMaxError"]);
		}

		LOG_INFO("主题应用: %s", name.c_str());
	}

	void Settings::DrawMain()
	{

		ImGui::BeginGroupPanel("菜单");
		{
			ConfigWidget(f_MenuKey, false,
				"切换主菜单可见性的热键. 不能为空!!!\n"\
				"如果忘记了这个键, 可以在配置文件中查看或设置它.");
			ConfigWidget(f_HotkeysEnabled, "启用热键.");
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("日志记录");
		{
			bool consoleChanged = ConfigWidget(f_ConsoleLogging,
				"启用控制台记录日志信息(更改将在重新启动后生效)");
			if (consoleChanged && !f_ConsoleLogging)
			{
				Logger::SetLevel(Logger::Level::None, Logger::LoggerType::ConsoleLogger);
			}

			bool fileLogging = ConfigWidget(f_FileLogging,
				"启用文件日志记录(更改将在重新启动后生效).\n" \
				"app目录下将创建一个文件夹用于存储日志.");
			if (fileLogging && !f_FileLogging)
			{
				Logger::SetLevel(Logger::Level::None, Logger::LoggerType::FileLogger);
			}
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("状态窗口");
		{
			ConfigWidget(f_StatusShow);
			ConfigWidget(f_StatusMove, "允许移动状态窗口.");
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("信息窗口");
		{
			ConfigWidget(f_InfoShow);
			ConfigWidget(f_InfoMove, "允许移动信息窗口.");
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("FPS指示器");
		{
			ConfigWidget(f_FpsShow);
			ConfigWidget(f_FpsMove, "允许移动FPS指示器.");
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("显示通知");
		{
			ConfigWidget(f_NotificationsShow, "窗口右下角将显示通知.");
			ConfigWidget(f_NotificationsDelay, 1, 1, 10000, "两次通知之间的延迟时间, 单位为毫秒.");
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("快速退出");
		{
			ConfigWidget("启用",
				f_FastExitEnable,
				"启用快速退出.\n"
			);
			if (!f_FastExitEnable)
				ImGui::BeginDisabled();

			ConfigWidget("热键", f_HotkeyExit, true,
				"退出游戏的按键.");

			if (!f_FastExitEnable)
				ImGui::EndDisabled();
		}
		ImGui::EndGroupPanel();

		ImGui::BeginGroupPanel("界面定制");
		{
			ImGui::SetNextItemWidth(200);
			if (ConfigWidget(f_FontSize, 1, 8, 64, "调整界面字体大小."))
				renderer::SetGlobalFontSize(f_FontSize);

			static std::string themeNameBuffer_;
			
			ImGui::SetNextItemWidth(200);
		    ImGui::InputText("主题名称", &themeNameBuffer_);

			const bool alreadyExist = m_Themes.contains(themeNameBuffer_);

			ImGui::SameLine();
			if (ImGui::Button(alreadyExist ? "替换主题" : "保存主题"))
			{
				ImGui::SameLine();
				if (themeNameBuffer_.empty())
					ImGui::Text("主题名称无效. 回到默认主题");
				ThemeExport(themeNameBuffer_);
				hasLoaded = false;
				f_DefaultTheme = themeNameBuffer_;
				Init();
				themeNameBuffer_.clear();
			}

			ImGui::SetNextItemWidth(200);
			if (ImGui::BeginCombo("主题选择", f_DefaultTheme.value().c_str()))
			{
				for (const auto& themeName : m_Themes | std::views::keys)
				{
					const bool isSelected = f_DefaultTheme.value() == themeName;
					if (ImGui::Selectable(themeName.c_str(), isSelected))
					{
						ApplyTheme(themeName);
						f_DefaultTheme = themeName;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			
			ImGui::SameLine();
			if (ImGui::Button("删除主题"))
			{
				std::filesystem::remove(themesDir / (f_DefaultTheme.value() + ".json"));
				LOG_INFO("删除主题: %s", f_DefaultTheme.value().c_str());
				f_DefaultTheme = "";
				hasLoaded = false;
				Init();
			}

			ConfigWidget(f_ShowStyleEditor, "显示ImGui主题定制窗口.");
		}
		ImGui::EndGroupPanel();
	}

	Settings* Settings::GetInstance()
	{
		static Settings instance;
		return &instance;
	}

	void Settings::OnExitKeyPressed()
	{
		if (!f_FastExitEnable || CheatManagerBase::IsMenuShowed())
			return;

		ExitProcess(0);
	}
}
