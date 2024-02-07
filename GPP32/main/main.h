#pragma once
#include <windows.h>
#include <future>
#include <thread>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <mutex>
#include <vector>
#include <cstdint>
#include <map>

#include "../library/json.hpp"
#include "../library/detours/HookManager.h"
#include "../library/imgui/imgui.h"
#include "../library/imgui/imgui_internal.h"
#include "../library/UnityResolve.hpp"
#include "../library/imgui/implot.h"
#include "../library/magic_enum/magic_enum_all.hpp"
#include "../Library/HotKey.hpp"
#include "../Library/Console.hpp"

#define U8(X) reinterpret_cast<const char*>(X)
#define ERROR() LOG_ERROR(std::format("code:{}\n", GetLastError()));

using I = UnityResolve;
using IM = UnityResolve::Method;
using IC = UnityResolve::Class;
using IT = UnityResolve::Type;
using IF = UnityResolve::Field;
using IA = UnityResolve::Assembly;
using II = UnityResolve::UnityType;
using H = HookManager;

class Main {
public:
    static auto Init(HMODULE hModule) -> void;
    static auto InitFeatures() -> void;
protected:
    inline static bool init;
    inline static bool show;
    inline static bool tips;
    inline static int upDateSpeed = 16;
    inline static int windowHeight;
    inline static int windowWidth;
    inline static HMODULE hModule;
    inline static std::wstring dllPath;
};

class Feature : public Main {
public:
    Feature(const Feature&) = delete;
    auto operator=(const Feature&) -> void = delete;

    struct GuiInfo {
        std::string tableName;
        std::string groupName;
        bool        needGroup;
        bool        needUpdate;
        bool        needDraw;
    };

    virtual auto GetInfo() const -> const GuiInfo & { return *new GuiInfo{ reinterpret_cast<const char*>("未定义"), reinterpret_cast<const char*>(""), false, false, false }; } ;
    virtual auto Draw() -> void { };
    virtual auto Render() -> void {};
    virtual auto Update() -> void {};
    virtual auto Save(nlohmann::json& json) -> void {};
    virtual auto Load(nlohmann::json& json) -> void {};

    inline static std::unordered_map<std::string, std::vector<Feature*>> features;

protected:
    ~Feature() = default;
    Feature() = default;
};

namespace DrawHelp {
	struct BoxScreen {
		ImVec2 lowerTopLeft;
		ImVec2 lowerTopRight;
		ImVec2 lowerBottomLeft;
		ImVec2 lowerBottomRight;

		ImVec2 upperTopLeft;
		ImVec2 upperTopRight;
		ImVec2 upperBottomLeft;
		ImVec2 upperBottomRight;

		static auto ToVec2(II::Vector3 vector3) -> ImVec2 {
			return { vector3.x, vector3.y };
		}
	};

	enum class OutlineSide : uint32_t {
		Left = 1,
		Right = 2,
		Top = 4,
		Bottom = 8,
		All = Left | Right | Top | Bottom
	};

	static auto DrawQuadLines(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImU32 col) -> void {
		const auto draw = ImGui::GetBackgroundDrawList();
		draw->AddLine(p1, p2, col);
		draw->AddLine(p2, p3, col);
		draw->AddLine(p3, p4, col);
		draw->AddLine(p4, p1, col);
	}

	static bool operator&(OutlineSide lhs, OutlineSide rhs) {
		return
			static_cast<std::underlying_type_t<OutlineSide>>(lhs) &
			static_cast<std::underlying_type_t<OutlineSide>>(rhs);
	}

	static auto DrawTextWithOutline(ImDrawList* drawList, const ImFont* font, const float fontSize, const ImVec2& screenPos, const char* text, const ImColor& textColor, const float outlineThickness, const OutlineSide sides, const ImColor& outlineColor) -> void {
		if (outlineThickness == 0.0f) drawList->AddText(font, fontSize, screenPos, outlineColor, text);
		else {
			if (sides & OutlineSide::Left)
				drawList->AddText(font, fontSize, { screenPos.x - outlineThickness, screenPos.y }, outlineColor, text);

			if (sides & OutlineSide::Right)
				drawList->AddText(font, fontSize, { screenPos.x + outlineThickness, screenPos.y }, outlineColor, text);

			if (sides & OutlineSide::Bottom)
				drawList->AddText(font, fontSize, { screenPos.x, screenPos.y - outlineThickness }, outlineColor, text);

			if (sides & OutlineSide::Top)
				drawList->AddText(font, fontSize, { screenPos.x, screenPos.y + outlineThickness }, outlineColor, text);
		}

		drawList->AddText(font, fontSize, screenPos, textColor, text);
	}

	static auto DrawTextWithOutline(ImDrawList* drawList, const ImVec2& screenPos, const char* text, const ImColor& textColor, const float outlineThickness, const OutlineSide sides, const ImColor& outlineColor) -> void { DrawTextWithOutline(drawList, nullptr, 0.0f, screenPos, text, textColor, outlineThickness, sides, outlineColor); }

	static auto ShowHelpText(const char* text) -> void {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	static auto HelpMarker(const char* desc) -> void {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) ShowHelpText(desc);
	}

	static auto AddUnderLine(const ImColor& col_) -> void {
		auto       min = ImGui::GetItemRectMin();
		const auto max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
	}

	static auto TextURL(const char* name, const wchar_t* url, const bool SameLineBefore_, const bool SameLineAfter_) -> void {
		if (SameLineBefore_) ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::Text(name);
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered()) {
			if (ImGui::IsMouseClicked(0))
				ShellExecute(nullptr, nullptr, url, nullptr, nullptr, SW_SHOW);
			AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
			ImGui::SetTooltip("Open in browser\n%s", url);
		}
		else AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
		if (SameLineAfter_) ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	}

	static ImColor GetTeamCol(const int teamId, int a) {
		switch (teamId) {
			case 1: {
				return { 32, 230, 193, a };
			}
			case 2: {
				return { 252, 81, 133, a };
			}
			case 3: {
				return { 234, 125, 198, a };
			}
			case 4: {
				return { 48, 137, 252, a };
			}
			case 5: {
				return { 255, 206, 243, a };
			}
			default: ;
		}
	}
};
