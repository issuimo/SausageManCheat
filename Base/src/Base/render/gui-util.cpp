#include "gui-util.h"

#include <shellapi.h>
#include <Base/util.h>

auto ShowHelpText(const char* text) -> void {
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	ImGui::TextUnformatted(text);
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}

auto HelpMarker(const char* desc) -> void {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) { ShowHelpText(desc); }
}

auto InputHotkey(const char* label, Hotkey* hotkey, const bool clearable) -> bool {
	char hotkeyBuffer[50];

	const auto hotkeyString = std::string(*hotkey);
	memcpy(hotkeyBuffer, hotkeyString.c_str(), hotkeyString.size() + 1);

	bool changed = false;

	if (clearable) {
		char labelBuffer[128];
		std::snprintf(labelBuffer, 128, "清除 ## %s_1", label);

		if (ImGui::Button(labelBuffer, ImVec2(75, 0))) {
			*hotkey = Hotkey();
			changed = true;
		}
		ImGui::SameLine();
	}

	changed = ImGui::HotkeyWidget(label, *hotkey, ImVec2(200, 0)) || changed;

	return changed;
}

auto CalcWidth(const std::string_view& view) -> float {
	const ImGuiContext& g = *GImGui;
	return ImGui::CalcTextSize(view.data()).x + g.Style.FramePadding.x * 2.0f + 25.0f;
}


#define END_TYPE_WIDGET() \
    if (desc != nullptr) { ImGui::SameLine(); HelpMarker(desc); } \
    return result;

#define END_CONFIG_WIDGET() if (result) field.FireChanged(); return result;

auto TypeWidget(const char* label, bool& value, const char* desc) -> bool {
	const bool result = ImGui::Checkbox(label, &value);
	END_TYPE_WIDGET();
}

auto TypeWidget(const char* label, int& value, const int step, const int start, const int end, const char* desc) -> bool {
	bool result = false;

	if (start == end) { result = ImGui::InputInt(label, &value, step); } else { result = ImGui::DragInt(label, &value, static_cast<float>(step), start, end, nullptr, ImGuiSliderFlags_AlwaysClamp); }

	END_TYPE_WIDGET();
}

auto TypeWidget(const char* label, float& value, const float step, const float start, const float end, const char* desc) -> bool {
	bool result = false;

	if (start == end) { result = ImGui::InputFloat(label, &value, step); } else { result = ImGui::DragFloat(label, &value, step, start, end, nullptr, ImGuiSliderFlags_AlwaysClamp); }

	END_TYPE_WIDGET();
}

auto TypeWidget(const char* label, Hotkey& value, const bool clearable, const char* desc) -> bool {
	const bool result = InputHotkey(label, &value, clearable);
	END_TYPE_WIDGET();
}

auto TypeWidget(const char* label, std::string& value, const char* desc) -> bool {
	const bool result = ImGui::InputText(label, &value);
	END_TYPE_WIDGET();
}

auto TypeWidget(const char* label, ImColor& value, const char* desc) -> bool {
	const bool result = ImGui::ColorEdit4(label, reinterpret_cast<float*>(&value));
	END_TYPE_WIDGET();
}

auto TypeWidget(const char* label, config::Toggle<Hotkey>& value, const char* desc, const bool hotkey) -> bool {
	const bool result = hotkey ? InputHotkey(label, &value.value, true) : ImGui::Checkbox(label, &value.enabled);
	END_TYPE_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<bool>& field, const char* desc) -> bool {
	const bool result = TypeWidget(label, field.value(), desc);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<int>& field, const int step, const int start, const int end, const char* desc) -> bool {
	const bool result = TypeWidget(label, field.value(), step, start, end, desc);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<float>& field, const float step, const float start, const float end, const char* desc) -> bool {
	const bool result = TypeWidget(label, field.value(), step, start, end, desc);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<Hotkey>& field, const bool clearable, const char* desc) -> bool {
	const bool result = TypeWidget(label, field.value(), clearable, desc);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<std::string>& field, const char* desc) -> bool {
	const bool result = TypeWidget(label, field.value(), desc);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<ImColor>& field, const char* desc /*= nullptr*/) -> bool {
	const bool result = TypeWidget(label, field.value(), desc);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<config::Toggle<float>>& field, const float step, const float start, const float end, const char* desc, bool hotkey) -> bool {
	ImGui::PushID(&label);
	bool result = TypeWidget("", field.value().enabled);
	ImGui::SameLine();
	result |= TypeWidget(label, field.value().value, step, start, end, desc);
	ImGui::PopID();
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const char* label, const config::Field<config::Toggle<Hotkey>>& field, const char* desc /*= nullptr*/, const bool hotkey /*= false*/) -> bool {
	const bool result = TypeWidget(label, field.value(), desc, hotkey);
	END_CONFIG_WIDGET();
}

auto ConfigWidget(const config::Field<bool>& field, const char* desc) -> bool { return ConfigWidget(field.friendName().c_str(), field, desc); }

auto ConfigWidget(const config::Field<int>& field, const int step, const int start, const int end, const char* desc) -> bool { return ConfigWidget(field.friendName().c_str(), field, step, start, end, desc); }

auto ConfigWidget(const config::Field<float>& field, const float step, const float start, const float end, const char* desc) -> bool { return ConfigWidget(field.friendName().c_str(), field, step, start, end, desc); }

auto ConfigWidget(const config::Field<Hotkey>& field, const bool clearable, const char* desc) -> bool { return ConfigWidget(field.friendName().c_str(), field, clearable, desc); }

auto ConfigWidget(const config::Field<std::string>& field, const char* desc) -> bool { return ConfigWidget(field.friendName().c_str(), field, desc); }

auto ConfigWidget(const config::Field<ImColor>& field, const char* desc /*= nullptr*/) -> bool { return ConfigWidget(field.friendName().c_str(), field, desc); }

auto ConfigWidget(const config::Field<config::Toggle<float>>& field, const float step, const float start, const float end, const char* desc) -> bool { return ConfigWidget(field.friendName().c_str(), field, step, start, end, desc); }

auto ConfigWidget(const config::Field<config::Toggle<Hotkey>>& field, const char* desc /*= nullptr*/, const bool hotkey /*= false*/) -> bool { return ConfigWidget(field.friendName().c_str(), field, desc, hotkey); }

#undef ShowDesc


auto AddUnderLine(const ImColor col_) -> void {
	ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();
	min.y = max.y;
	ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
}

auto TextURL(const char* name_, const char* URL_, const bool SameLineBefore_, const bool SameLineAfter_) -> void {
	if (SameLineBefore_) ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
	ImGui::Text(name_);
	ImGui::PopStyleColor();
	if (ImGui::IsItemHovered()) {
		if (ImGui::IsMouseClicked(0)) { ShellExecuteA(nullptr, nullptr, URL_, nullptr, nullptr, SW_SHOW); }
		AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
		ImGui::SetTooltip(reinterpret_cast<const char*>(u8"浏览器中打开\n%s"), URL_);
	} else { AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]); }
	if (SameLineAfter_) ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
}

auto operator&(OutlineSide lhs, OutlineSide rhs) -> bool { return static_cast<std::underlying_type_t<OutlineSide>>(lhs) & static_cast<std::underlying_type_t<OutlineSide>>(rhs); }

auto DrawTextWithOutline(ImDrawList* drawList, const ImFont* font, const float fontSize, const ImVec2& screenPos, const char* text, const ImColor& textColor, const float outlineThickness, const OutlineSide sides, const ImColor& outlineColor) -> void {
	if (outlineThickness == 0.0f) { drawList->AddText(font, fontSize, screenPos, outlineColor, text); } else {
		if (sides & OutlineSide::Left) { drawList->AddText(font, fontSize, {screenPos.x - outlineThickness, screenPos.y}, outlineColor, text); }

		if (sides & OutlineSide::Right) { drawList->AddText(font, fontSize, {screenPos.x + outlineThickness, screenPos.y}, outlineColor, text); }

		if (sides & OutlineSide::Bottom) { drawList->AddText(font, fontSize, {screenPos.x, screenPos.y - outlineThickness}, outlineColor, text); }

		if (sides & OutlineSide::Top) { drawList->AddText(font, fontSize, {screenPos.x, screenPos.y + outlineThickness}, outlineColor, text); }
	}

	drawList->AddText(font, fontSize, screenPos, textColor, text);
}

auto DrawTextWithOutline(ImDrawList* drawList, const ImVec2& screenPos, const char* text, const ImColor& textColor, const float outlineThickness, const OutlineSide sides, const ImColor& outlineColor) -> void { DrawTextWithOutline(drawList, nullptr, 0.0f, screenPos, text, textColor, outlineThickness, sides, outlineColor); }

// Modified version of: https://github.com/spirthack/CSGOSimple/blob/master/CSGOSimple/UI.cpp#L287 
auto ImGui::HotkeyWidget(const char* label, Hotkey& hotkey, const ImVec2& size) -> bool {
	// Init ImGui
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems) { return false; }

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	const ImVec2 label_size = CalcTextSize(label, nullptr, true);
	const ImVec2 item_size = CalcItemSize(size, CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);

	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + item_size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id)) { return false; }


	const bool focus_requested = (GetItemStatusFlags() & ImGuiItemStatusFlags_Checked) != 0 || g.NavId == id;
	const bool hovered = ItemHoverable(frame_bb, id, ImGuiItemStatusFlags_None);
	if (hovered) {
		SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	static Hotkey _initHotkey;
	static Hotkey _currHotkey;
	static Hotkey _prevHotkey;

	if (const bool user_clicked = hovered && io.MouseClicked[0]; user_clicked) {
		if (g.ActiveId != id) {
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));

			_initHotkey = hotkey;
			_currHotkey = Hotkey();
			_prevHotkey = Hotkey();
		}

		SetActiveID(id, window);
		FocusWindow(window);
	} else if (io.MouseClicked[0] && g.ActiveId == id) { ClearActiveID(); }

	bool valueChanged = false;

	if (g.ActiveId == id) {
		if (IsKeyPressed(ImGuiKey_Escape)) {
			ClearActiveID();
			if (hotkey != _initHotkey) {
				hotkey = _initHotkey;
				valueChanged = true;
			}
		} else {
			NavMoveRequestCancel();

			auto newHotkey = Hotkey::GetPressedHotkey();

			if (newHotkey.IsEmpty() && !_currHotkey.IsEmpty()) {
				ClearActiveID();
				valueChanged = false;
			} else if (newHotkey - _prevHotkey) {
				_currHotkey = newHotkey;
				hotkey = newHotkey;
				valueChanged = true;
			}

			_prevHotkey = newHotkey;
		}
	}

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[128] = "Empty";

	const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

	if ((g.ActiveId == id && !_currHotkey.IsEmpty()) || g.ActiveId != id) { strcpy_s(buf_display, static_cast<std::string>(hotkey).c_str()); } else if (g.ActiveId == id) { strcpy_s(buf_display, "<Press a key>"); }

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + item_size.x, frame_bb.Min.y + item_size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, nullptr, nullptr, style.ButtonTextAlign, &clip_rect);
	//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
	//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);
	if (label_size.x > 0) { RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label); }

	return valueChanged;
}

// https://github.com/ocornut/imgui/issues/3798
auto ImGui::CalcContrastRatio(const ImU32& backgroundColor, const ImU32& foreGroundColor) -> float {
	// real code https://www.w3.org/TR/WCAG20/#relativeluminancedef
	/*const auto colBG = ImGui::ColorConvertU32ToFloat4(backgroundColor);
	const auto colFG = ImGui::ColorConvertU32ToFloat4(foreGroundColor);
	float lumBG = 0.2126 * colBG.x + 0.7152 * colBG.y + 0.0722 * colBG.z;
	float lumFG = 0.2126 * colFG.x + 0.7152 * colFG.y + 0.0722 * colFG.z;
	return (ImMax(lumBG, lumFG) + 0.05) / (ImMin(lumBG, lumFG) + 0.05);*/

	const float sa0 = static_cast<float>((backgroundColor >> IM_COL32_A_SHIFT) & 0xFF);
	const float sa1 = static_cast<float>((foreGroundColor >> IM_COL32_A_SHIFT) & 0xFF);
	static float sr = 0.2126f / 255.0f;
	static float sg = 0.7152f / 255.0f;
	static float sb = 0.0722f / 255.0f;
	const float contrastRatio = (sr * sa0 * ((backgroundColor >> IM_COL32_R_SHIFT) & 0xFF) + sg * sa0 * ((backgroundColor >> IM_COL32_G_SHIFT) & 0xFF) + sb * sa0 * ((backgroundColor >> IM_COL32_B_SHIFT) & 0xFF) + 0.05f) / (sr * sa1 * ((foreGroundColor >> IM_COL32_R_SHIFT) & 0xFF) + sg * sa1 * ((foreGroundColor >> IM_COL32_G_SHIFT) & 0xFF) + sb * sa1 * ((foreGroundColor >> IM_COL32_B_SHIFT) & 0xFF) + 0.05f);
	if (contrastRatio < 1.0f) { return 1.0f / contrastRatio; }
	return contrastRatio;
}

auto ImGui::CalcContrastColor(const ImColor& foreground, const float maxContrastRatio, const ImColor& background, const ImColor& inverted) -> ImColor { return CalcContrastRatio(background, foreground) < maxContrastRatio ? inverted : background; }

auto ImGui::PushStyleColorWithContrast(const ImU32 backGroundColor, const ImGuiCol foreGroundColor, const ImU32 invertedColor, const float maxContrastRatio) -> bool {
	const float contrastRatio = CalcContrastRatio(backGroundColor, GetColorU32(foreGroundColor));
	if (contrastRatio < maxContrastRatio) {
		PushStyleColor(foreGroundColor, invertedColor);
		return true;
	}
	return false;
}

static std::string nameBuffer;

auto ImGui::OpenRenamePopup(const std::string& initName) -> void {
	OpenPopup("RenamePopup");
	if (IsRenamePopupOpened()) { nameBuffer = initName; }
}

auto ImGui::IsRenamePopupOpened() -> bool { return IsPopupOpen("RenamePopup"); }

auto ImGui::DrawRenamePopup(std::string& out) -> bool {
	if (BeginPopup("RenamePopup", ImGuiWindowFlags_AlwaysAutoResize)) {
		Text("To save press `Enter`.\nTo close without saving press `Esc`.");

		if (!IsAnyItemActive() && !IsMouseClicked(0)) { SetKeyboardFocusHere(0); }

		InputText("Name", &nameBuffer);

		bool changed = false;
		if (IsKeyPressed(ImGuiKey_Enter, false)) {
			changed = true;
			out = nameBuffer;
			CloseCurrentPopup();
		}

		if (IsKeyPressed(ImGuiKey_Escape, false)) { CloseCurrentPopup(); }

		EndPopup();
		return changed;
	}
	return false;
}

namespace ImGui {
	struct GroupPanelHeaderBounds {
		ImRect left;
		ImRect right;
		bool collapsed;
	};

	static ImVector<GroupPanelHeaderBounds> _groupPanelStack;

	static auto GroupPanelIsOpen(const ImGuiID id) -> bool {
		const ImGuiContext& g = *GImGui;
		const ImGuiWindow* window = g.CurrentWindow;
		const ImGuiStorage* storage = window->DC.StateStorage;

		return storage->GetInt(id, 1) != 0;
	}

	static auto GroupPanelSetOpen(const ImGuiID id, const bool open) -> void {
		const ImGuiContext& g = *GImGui;
		const ImGuiWindow* window = g.CurrentWindow;
		ImGuiStorage* storage = window->DC.StateStorage;

		storage->SetInt(id, open ? 1 : 0);
	}

	// Modified version of: https://github.com/ocornut/imgui/issues/1496#issuecomment-655048353

	auto BeginGroupPanel(const char* label, const bool node, const ImVec2& size) -> bool {
		const ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		const ImGuiID id = window->GetID(label);
		PushID(id);

		auto groupPanelPos = window->DC.CursorPos;
		const auto itemSpacing = GetStyle().ItemSpacing;

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		BeginGroup(); // Outer group

		ImVec2 effectiveSize = size;
		if (size.x < 0.0f) { effectiveSize.x = GetContentRegionAvail().x; } else { effectiveSize.x = size.x; }

		Dummy(ImVec2(effectiveSize.x, 0.0f)); // Adjusting group x size

		const auto frameHeight = GetFrameHeight();
		Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
		SameLine(0.0f, 0.0f); // Inner group spacing
		BeginGroup();         // Inner group

		Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
		SameLine(0.0f, 0.0f); // Name text spacing
		TextUnformatted(label);

		ImRect leftRect = {GetItemRectMin(), GetItemRectMax()};
		auto rightMax = ImVec2(groupPanelPos.x + effectiveSize.x - frameHeight, leftRect.Max.y);
		const ImRect rightRect = {{rightMax.x, leftRect.Min.x}, rightMax};
		SameLine(0.0f, 0.0f);

		Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));

		if (node) {
			leftRect.Min.x = groupPanelPos.x;

			const ImVec2 text_size = CalcTextSize(label);
			bool isOpen = GroupPanelIsOpen(id);

			bool hovered;
			const bool toggled = ButtonBehavior(leftRect, id, &hovered, nullptr, ImGuiButtonFlags_PressedOnClick);
			if (toggled) {
				isOpen = !isOpen;
				GroupPanelSetOpen(id, isOpen);
			}

			const ImU32 text_col = GetColorU32(ImGuiCol_Text);
			RenderArrow(window->DrawList, {groupPanelPos.x, groupPanelPos.y + text_size.y * 0.15f}, text_col, isOpen ? ImGuiDir_Down : ImGuiDir_Right, 0.7f);

			if (!isOpen) {
				PopStyleVar(2);
				EndGroup();
				EndGroup();
				PopID();

				_groupPanelStack.push_back({leftRect, rightRect, true});
				return false;
			}
		}

		PopStyleVar(2);

		GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
		GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
		GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
		GetCurrentWindow()->Size.x -= frameHeight;

		const auto itemWidth = CalcItemWidth();
		PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

		_groupPanelStack.push_back({leftRect, rightRect, false});
		return true;
	}

	auto EndGroupPanel() -> void {
		IM_ASSERT(_groupPanelStack.Size > 0); // Mismatched BeginGroupPanel()/EndGroupPanel() calls
		auto& info = _groupPanelStack.back();
		_groupPanelStack.pop_back();

		if (info.collapsed) { return; }

		PopItemWidth();

		const auto itemSpacing = GetStyle().ItemSpacing;
		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		EndGroup(); // Inner group

		const auto frameHeight = GetFrameHeight();
		SameLine(0.0f, 0.0f);
		Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
		Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

		EndGroup(); // Outer group

		// Outer group rect
		auto itemMin = GetItemRectMin();
		auto itemMax = GetItemRectMax();

		ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
		const auto frameRect = ImRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f));

		auto& leftRect = info.left;
		leftRect.Min.x -= itemSpacing.x;
		leftRect.Max.x += itemSpacing.x;

		const bool hasRightPart = info.right.Min.x != info.right.Max.x;
		auto& rightRect = info.right;

		if (hasRightPart) {
			rightRect.Min.x -= itemSpacing.x;
			rightRect.Max.x += itemSpacing.x;
		}

		// Drawing rectangle
		for (int i = 0; i < (hasRightPart ? 5 : 3); ++i) {
			switch (i) {
				// left half-plane
				case 0: PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(leftRect.Min.x, FLT_MAX), true);
					break;
				// right half-plane
				case 1: PushClipRect(ImVec2(leftRect.Max.x, -FLT_MAX), ImVec2(hasRightPart ? rightRect.Min.x : FLT_MAX, FLT_MAX), true);
					break;
				// bottom
				case 2: PushClipRect(ImVec2(leftRect.Min.x, leftRect.Max.y), ImVec2(leftRect.Max.x, FLT_MAX), true);
					break;
				// bottom select
				case 3: PushClipRect(ImVec2(rightRect.Min.x, rightRect.Max.y), ImVec2(rightRect.Max.x, FLT_MAX), true);
					break;
				// right hand-plane
				case 4: PushClipRect(ImVec2(rightRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true);
					break;
			}

			GetWindowDrawList()->AddRect(frameRect.Min, frameRect.Max, ImColor(GetStyleColorVec4(ImGuiCol_Border)), halfFrame.x);

			PopClipRect();
		}


		PopStyleVar(2);

		// Restore content region
		GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
		GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
		GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
		GetCurrentWindow()->Size.x += frameHeight;

		// Add vertical spacing
		Dummy(ImVec2(0.0f, 0.0f));

		PopID();
	}

	auto NextGroupPanelHeaderItem(const ImVec2& size, const bool rightAlign) -> void {
		IM_ASSERT(size.x > 0.0f);             // Size should be specified
		IM_ASSERT(_groupPanelStack.Size > 0); // Mismatched BeginGroupPanel()/EndGroupPanel() calls
		auto& info = _groupPanelStack.back();

		const ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;

		if (rightAlign) {
			if (info.right.Min.x != info.right.Max.x) { info.right.Min.x -= g.Style.ItemSpacing.x; }

			info.right.Min.x -= size.x;
		} else { info.left.Max.x += g.Style.ItemSpacing.x; }

		window->DC.CursorPos.x = rightAlign ? info.right.Min.x : info.left.Max.x;
		window->DC.CursorPos.y = info.left.Min.y - (size.y - GetFrameHeight() + g.Style.FramePadding.y) / 2;

		if (!rightAlign) { info.left.Max.x += size.x; }
	}

	auto BeginSelectableGroupPanel(const char* label, bool& value, bool& changed, const bool node, const ImVec2& size, const char* selectLabel) -> bool {
		const bool opened = BeginGroupPanel(label, node, size);

		PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));

		const ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		const ImVec2 label_size = CalcTextSize(selectLabel, nullptr, true);
		const float square_sz = GetFrameHeight();
		const auto checkbox_size = ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f);

		NextGroupPanelHeaderItem(checkbox_size, true);
		changed = Checkbox(selectLabel, &value);

		PopStyleVar();
		return opened;
	}

	auto EndSelectableGroupPanel() -> void { EndGroupPanel(); }

	auto CalcButtonSize(const char* label) -> ImVec2 {
		const ImVec2 label_size = CalcTextSize(label, nullptr, true);

		const ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImVec2 size = CalcItemSize({}, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
		return size;
	}
}

void DrawOffscreenArrows(const ImVec4& pos, const ImColor& color, const float arrowRadius, const float tracerSize, const float outlineThickness) {
	const ImRect screen_rect = { 0.0f, 0.0f, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y };
	if (pos.w == 1.0f && (pos.z > 0 && screen_rect.Contains({ pos.x, pos.y })))
		return;

	const auto screen_center = screen_rect.GetCenter();
	auto angle = atan2(screen_center.y - pos.y, screen_center.x - pos.x);
	angle += pos.z > 0 ? PI : 0.0f;

	const ImVec2 arrow_center{
		screen_center.x + arrowRadius * cosf(angle),
		screen_center.y + arrowRadius * sinf(angle)
	};

	// Triangle
	std::array<ImVec2, 4> points{
		ImVec2(-22.0f, -8.6f),
		ImVec2(0.0f, 0.0f),
		ImVec2(-22.0f, 8.6f),
		ImVec2(-18.0f, 0.0f)
	};

	for (auto& point : points) {
		const auto x = point.x * tracerSize;
		const auto y = point.y * tracerSize;

		point.x = arrow_center.x + x * cosf(angle) - y * sinf(angle);
		point.y = arrow_center.y + x * sinf(angle) + y * cosf(angle);
	}


	const auto draw = ImGui::GetBackgroundDrawList();

	float alpha = 1.0f;
	if (pos.z > 0) {
		constexpr float nearThreshold     = 200.0f * 200.0f;
		const ImVec2    screen_outer_diff = {
			pos.x < 0 ? abs(pos.x) : (pos.x > screen_rect.Max.x ? pos.x - screen_rect.Max.x : 0.0f),
			pos.y < 0 ? abs(pos.y) : (pos.y > screen_rect.Max.y ? pos.y - screen_rect.Max.y : 0.0f),
		};
		const float distance = static_cast<float>(std::pow(screen_outer_diff.x, 2) + std::pow(screen_outer_diff.y, 2));
		alpha = pos.z < 0 ? 1.0f : (distance / nearThreshold);
	}
	auto arrowColor = color; // Copy
	arrowColor.Value.w = min(alpha, 1.0f);

	// Draw the arrow
	draw->AddTriangleFilled(points[0], points[1], points[3], arrowColor);
	draw->AddTriangleFilled(points[2], points[1], points[3], arrowColor);
	// draw->AddQuad(points[0], points[1], points[2], points[3], ImColor(0.0f, 0.0f, 0.0f, alpha), 0.6f);
	draw->AddQuad(points[0], points[1], points[2], points[3], ImColor(0.0f, 0.0f, 0.0f, alpha), outlineThickness);
}