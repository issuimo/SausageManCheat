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

	static auto GetTeamColor(const unsigned long teamId, const unsigned long a) -> ImColor {
        switch (teamId) {
	        case 0: return {0, 0, 0, a};
	        case 1: return {133, 151, 21, a};
	        case 2: return {173, 29, 210, a};
	        case 3: return {148, 221, 196, a};
	        case 4: return {118, 25, 57, a};
	        case 5: return {49, 241, 173, a};
	        case 6: return {181, 88, 240, a};
	        case 7: return {147, 151, 50, a};
	        case 8: return {25, 43, 209, a};
	        case 9: return {192, 253, 22, a};
	        case 10: return {142, 78, 72, a};
	        case 11: return {155, 11, 245, a};
	        case 12: return {59, 73, 168, a};
	        case 13: return {99, 93, 222, a};
	        case 14: return {63, 223, 109, a};
	        case 15: return {104, 180, 135, a};
	        case 16: return {154, 170, 205, a};
	        case 17: return {220, 247, 193, a};
	        case 18: return {68, 129, 41, a};
	        case 19: return {8, 27, 64, a};
	        case 20: return {98, 56, 48, a};
	        case 21: return {78, 148, 212, a};
	        case 22: return {17, 208, 222, a};
	        case 23: return {196, 17, 157, a};
	        case 24: return {75, 63, 156, a};
	        case 25: return {70, 187, 239, a};
	        case 26: return {199, 84, 33, a};
	        case 27: return {80, 43, 208, a};
	        case 28: return {239, 90, 244, a};
	        case 29: return {9, 207, 95, a};
	        case 30: return {53, 145, 148, a};
	        case 31: return {54, 127, 137, a};
	        case 32: return {112, 153, 177, a};
	        case 33: return {30, 103, 204, a};
	        case 34: return {17, 84, 3, a};
	        case 35: return {127, 156, 3, a};
	        case 36: return {74, 246, 155, a};
	        case 37: return {30, 237, 103, a};
	        case 38: return {119, 59, 194, a};
	        case 39: return {164, 206, 80, a};
	        case 40: return {116, 249, 198, a};
	        case 41: return {187, 122, 88, a};
	        case 42: return {162, 134, 69, a};
	        case 43: return {179, 147, 232, a};
	        case 44: return {190, 170, 208, a};
	        case 45: return {15, 239, 102, a};
	        case 46: return {232, 28, 0, a};
	        case 47: return {197, 87, 112, a};
	        case 48: return {102, 183, 88, a};
	        case 49: return {38, 87, 232, a};
	        case 50: return {251, 224, 129, a};
	        case 51: return {159, 119, 199, a};
	        case 52: return {251, 230, 193, a};
	        case 53: return {205, 124, 235, a};
	        case 54: return {94, 54, 203, a};
	        case 55: return {166, 117, 86, a};
	        case 56: return {118, 188, 40, a};
	        case 57: return {156, 199, 163, a};
	        case 58: return {36, 207, 244, a};
	        case 59: return {7, 119, 152, a};
	        case 60: return {150, 109, 163, a};
	        case 61: return {65, 92, 23, a};
	        case 62: return {240, 188, 1, a};
	        case 63: return {39, 6, 231, a};
	        case 64: return {123, 7, 186, a};
	        case 65: return {14, 118, 7, a};
	        case 66: return {41, 177, 0, a};
	        case 67: return {72, 3, 136, a};
	        case 68: return {134, 218, 42, a};
	        case 69: return {197, 255, 33, a};
	        case 70: return {124, 153, 103, a};
	        case 71: return {125, 236, 249, a};
	        case 72: return {111, 41, 216, a};
	        case 73: return {162, 115, 100, a};
	        case 74: return {151, 91, 172, a};
	        case 75: return {207, 81, 122, a};
	        case 76: return {167, 23, 19, a};
	        case 77: return {169, 245, 211, a};
	        case 78: return {34, 234, 37, a};
	        case 79: return {176, 144, 217, a};
	        case 80: return {239, 203, 14, a};
	        case 81: return {49, 178, 94, a};
	        case 82: return {1, 146, 33, a};
	        case 83: return {159, 216, 130, a};
	        case 84: return {56, 229, 156, a};
	        case 85: return {177, 118, 13, a};
	        case 86: return {200, 129, 70, a};
	        case 87: return {28, 92, 197, a};
	        case 88: return {190, 37, 45, a};
	        case 89: return {201, 139, 110, a};
	        case 90: return {127, 148, 44, a};
	        case 91: return {206, 213, 105, a};
	        case 92: return {95, 89, 8, a};
	        case 93: return {148, 217, 64, a};
	        case 94: return {151, 99, 58, a};
	        case 95: return {242, 62, 114, a};
	        case 96: return {64, 135, 202, a};
	        case 97: return {43, 59, 95, a};
	        case 98: return {43, 142, 193, a};
	        case 99: return {6, 203, 46, a};
	        case 100: return {162, 91, 30, a};
	        default:return 0;
        }
    }
};
