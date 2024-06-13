#pragma once

#include <pch.h>

#include <filesystem>
#include <Base/Hotkey.h>
#include <Base/Encode.h>
#include <Base/config/config.h>
#include <Base/config/fields/Enum.h>
#include <Base/config/fields/Toggle.h>

#define BLOCK_FOCUS() 

auto TypeWidget(const char* label, bool& value, const char* desc = nullptr) -> bool;
auto TypeWidget(const char* label, int& value, int step = 1, int start = 0, int end = 0, const char* desc = nullptr) -> bool;
auto TypeWidget(const char* label, float& value, float step = 1.0F, float start = 0, float end = 0, const char* desc = nullptr) -> bool;
auto TypeWidget(const char* label, Hotkey& value, bool clearable = true, const char* desc = nullptr) -> bool;
auto TypeWidget(const char* label, std::string& value, const char* desc = nullptr) -> bool;
auto TypeWidget(const char* label, ImColor& value, const char* desc = nullptr) -> bool;
auto TypeWidget(const char* label, config::Toggle<Hotkey>& value, const char* desc = nullptr, bool hotkey = false) -> bool;

auto ConfigWidget(const char* label, const config::Field<bool>& field, const char* desc = nullptr) -> bool;
auto ConfigWidget(const char* label, const config::Field<int>& field, int step = 1, int start = 0, int end = 0, const char* desc = nullptr) -> bool;
auto ConfigWidget(const char* label, const config::Field<float>& field, float step = 1.0F, float start = 0, float end = 0, const char* desc = nullptr) -> bool;
auto ConfigWidget(const char* label, const config::Field<Hotkey>& field, bool clearable = true, const char* desc = nullptr) -> bool;
auto ConfigWidget(const char* label, const config::Field<std::string>& field, const char* desc = nullptr) -> bool;
auto ConfigWidget(const char* label, const config::Field<ImColor>& field, const char* desc = nullptr) -> bool;

template <typename T>
auto ConfigWidget(const char* label, config::Field<config::Toggle<T>>& field, const char* desc = nullptr) -> bool {
	ImGui::PushID(&field);
	bool result = TypeWidget("", field.value().enabled);
	ImGui::SameLine();
	result |= TypeWidget(label, field.value().value, desc);
	ImGui::PopID();
	if (result) field.FireChanged();
	return result;
}

auto ConfigWidget(const char* label, const config::Field<config::Toggle<float>>& field, float step = 1.0F, float start = 0, float end = 0, const char* desc = nullptr, bool hotkey = false) -> bool;
auto ConfigWidget(const char* label, const config::Field<config::Toggle<Hotkey>>& field, const char* desc = nullptr, bool hotkey = false) -> bool;

auto ConfigWidget(const config::Field<bool>& field, const char* desc = nullptr) -> bool;
auto ConfigWidget(const config::Field<int>& field, int step = 1, int start = 0, int end = 0, const char* desc = nullptr) -> bool;
auto ConfigWidget(const config::Field<float>& field, float step = 1.0F, float start = 0, float end = 0, const char* desc = nullptr) -> bool;
auto ConfigWidget(const config::Field<Hotkey>& field, bool clearable = true, const char* desc = nullptr) -> bool;
auto ConfigWidget(const config::Field<std::string>& field, const char* desc = nullptr) -> bool;
auto ConfigWidget(const config::Field<ImColor>& field, const char* desc = nullptr) -> bool;

template <typename T>
auto ConfigWidget(config::Field<config::Toggle<T>>& field, const char* desc = nullptr) -> bool { return ConfigWidget(field.friendName().c_str(), field, desc); }

auto ConfigWidget(const config::Field<config::Toggle<float>>& field, float step = 1.0F, float start = 0, float end = 0, const char* desc = nullptr) -> bool;
auto ConfigWidget(const config::Field<config::Toggle<Hotkey>>& field, const char* desc = nullptr, bool hotkey = false) -> bool;

auto ShowHelpText(const char* text) -> void;
auto HelpMarker(const char* desc) -> void;

auto InputHotkey(const char* label, Hotkey* hotkey, bool clearable) -> bool;

// Thanks to https://gist.github.com/dougbinks/ef0962ef6ebe2cadae76c4e9f0586c69
auto AddUnderLine(ImColor col_) -> void;
auto TextURL(const char* name_, const char* URL_, bool SameLineBefore_, bool SameLineAfter_) -> void;

enum class OutlineSide : uint32_t {
	Left   = 1,
	Right  = 2,
	Top    = 4,
	Bottom = 8,
	All    = Left | Right | Top | Bottom
};

auto operator&(OutlineSide lhs, OutlineSide rhs) -> bool;
auto DrawTextWithOutline(ImDrawList* drawList, const ImFont* font, float fontSize, const ImVec2& screenPos, const char* text, const ImColor& textColor, float outlineThickness = 0.0f, OutlineSide sides = OutlineSide::All, const ImColor& outlineColor = ImColor(0.0f, 0.0f, 0.0f)) -> void;
auto DrawTextWithOutline(ImDrawList* drawList, const ImVec2& screenPos, const char* text, const ImColor& textColor, float outlineThickness = 0.0f, OutlineSide sides = OutlineSide::All, const ImColor& outlineColor = ImColor(0.0f, 0.0f, 0.0f)) -> void;

namespace ImGui {
	auto HotkeyWidget(const char* label, Hotkey& hotkey, const ImVec2& size = ImVec2(0, 0)) -> bool;

	auto CalcContrastRatio(const ImU32& backgroundColor, const ImU32& foreGroundColor) -> float;
	auto CalcContrastColor(const ImColor& foreground, float maxContrastRatio = 2.0f, const ImColor& background = ImColor(1.0f, 1.0f, 1.0f), const ImColor& inverted = ImColor(0.0f, 0.0f, 0.0f)) -> ImColor;
	auto PushStyleColorWithContrast(ImU32 backGroundColor, ImGuiCol foreGroundColor, ImU32 invertedColor, float maxContrastRatio) -> bool;

	auto OpenRenamePopup(const std::string& initName) -> void;
	auto IsRenamePopupOpened() -> bool;
	auto DrawRenamePopup(std::string& out) -> bool;

	auto BeginGroupPanel(const char* label, bool node = false, const ImVec2& size = ImVec2(-1.0f, 0.0f)) -> bool;
	auto EndGroupPanel() -> void;

	auto BeginSelectableGroupPanel(const char* label, bool& value, bool& changed, bool node = false, const ImVec2& size = ImVec2(-1.0f, 0.0f), const char* selectLabel = "Select") -> bool;
	auto EndSelectableGroupPanel() -> void;

	auto NextGroupPanelHeaderItem(const ImVec2& size, bool rightAlign = false) -> void;

	auto CalcButtonSize(const char* label) -> ImVec2;
}

auto CalcWidth(const std::string_view& view) -> float;

template <typename T>
auto GetMaxEnumWidth() -> float {
	constexpr auto names = magic_enum::enum_names<T>();
	auto maxComboName = std::max_element(names.begin(), names.end(), [](const auto& a, const auto& b) { return CalcWidth(a) < CalcWidth(b); });
	return CalcWidth(*maxComboName);
}

template <typename T>
auto ComboEnum(const char* label, T* currentValue, std::vector<T>* whitelist = nullptr) -> bool {
	auto name = magic_enum::enum_name(*currentValue);
	auto& current = *currentValue;
	bool result = false;
	static auto width = GetMaxEnumWidth<T>();

	std::unordered_set<T> whiteSet;
	if (whitelist != nullptr) { for (auto& value : *whitelist) { whiteSet.insert(value); } }
	ImGui::SetNextItemWidth(width);
	if (ImGui::BeginCombo(label, name.data())) {
		for (auto& entry : magic_enum::enum_entries<T>()) {
			if (whitelist != nullptr && !whiteSet.contains(entry.first)) { continue; }

			bool is_selected = (name == entry.second);
			if (ImGui::Selectable(entry.second.data(), is_selected)) {
				current = entry.first;
				result = true;
			}
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
		}
		ImGui::EndCombo();
	}
	return result;
}

template <typename T>
auto ConfigWidget(const char* label, config::Field<config::Enum<T>>& field, const char* desc = nullptr) -> bool {
	bool result = false;
	if (ComboEnum(label, &field.value())) {
		field.FireChanged();
		result = true;
	}

	if (desc != nullptr) {
		ImGui::SameLine();
		HelpMarker(desc);
	}
	return result;
}

template <typename T>
auto ConfigWidget(config::Field<config::Enum<T>>& field, const char* desc = nullptr) -> bool { return ConfigWidget(field.friendName().c_str(), field, desc); }

#define PI 3.14159265358979323846f
void DrawOffscreenArrows(const ImVec4& pos, const ImColor& color, const float arrowRadius, const float tracerSize, const float outlineThickness);