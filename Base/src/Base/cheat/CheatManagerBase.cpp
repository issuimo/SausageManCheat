#include <pch.h>
#include "CheatManagerBase.h"

#include <misc/cpp/imgui_stdlib.h>

#include <Base/render/renderer.h>
#include <Base/render/gui-util.h>
#include <Base/cheat/misc/settings.h>

#include <imgui_internal.h>

#include "fmt/color.h"

namespace cheat
{

	auto CheatManagerBase::Init(LPBYTE pFontData, DWORD dFontDataSize, renderer::DXVersion dxVersion) -> void {
		renderer::Init(pFontData, dFontDataSize, dxVersion);

		events::RenderEvent += MY_METHOD_HANDLER(CheatManagerBase::OnRender);
		events::KeyUpEvent += MY_METHOD_HANDLER(CheatManagerBase::OnKeyUp);
		events::WndProcEvent += MY_METHOD_HANDLER(CheatManagerBase::OnWndProc);
	}

	CheatManagerBase::CheatManagerBase():
		NF(m_SelectedSection, "", "General", 0),
		m_IsBlockingInput(true),
		m_IsPrevCursorActive(false)
	{
	}

	auto CheatManagerBase::DrawExternal() const -> void {
		for (auto& feature : m_Features) {
			ImGui::PushID(&feature);
			feature->DrawExternal();
			ImGui::PopID();
		}
	}

	auto CheatManagerBase::DrawMenu() -> void {
		if (m_ModuleOrder.empty()) {
			return;
		}

		static std::string* current = &m_ModuleOrder[m_SelectedSection];

		ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

		if (!ImGui::Begin("ATOM X UnityResolve.hpp"))
		{
			ImGui::End();
			return;
		}

		ImGui::BeginGroup();

		if (ImGui::Checkbox("阻止键鼠穿透", &m_IsBlockingInput))
		{
			renderer::SetInputLock(this, m_IsBlockingInput);
		}

		if (ImGui::BeginListBox("##listbox 2", ImVec2(175, -FLT_MIN)))
		{
			size_t index = 0;
			for (auto& moduleName : m_ModuleOrder)
			{
				const bool is_selected = (current == &moduleName);
				if (ImGui::Selectable(moduleName.c_str(), is_selected))
				{
					current = &moduleName;
					m_SelectedSection = index;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
				index++;
			}
			ImGui::TextColored(ImColor{ 50, 255, 50 }, "------------------");
			ImGui::Text("本程序完全免费禁止倒卖");
			ImGui::Text("本程序只在官方QQ群发布");
			ImGui::Text("其他渠道均为第三方搬运");
			ImGui::Text("搬运渠道不保证程序无毒");
			ImGui::Text("官方QQ群: 472659840");
			ImGui::Text("加入官方群更快获取更新");
			ImGui::Text("联系我请访问 遂沫.com");
			ImGui::EndListBox();
		}

		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();

		DrawProfileLine();

		constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 0), true, window_flags);

		auto& sections = m_FeatureMap[*current];
		const auto emptyName = std::string();
		if (sections.contains(emptyName)) {
			DrawMenuSection(emptyName, sections[""]);
		}

		for (auto& [sectionName, features] : sections)
		{
			if (sectionName.empty()) {
				continue;
			}

			DrawMenuSection(sectionName, features);
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::EndGroup();

		ImGui::End();
	}

	auto CheatManagerBase::DrawMenuSection(const std::string& sectionName, const std::vector<Feature*>& features) -> void {
		if (!sectionName.empty()) {
			ImGui::BeginGroupPanel(sectionName.c_str());
		}

		for (auto& feature : features)
		{
			ImGui::PushID(&feature);
			feature->DrawMain();
			ImGui::PopID();
		}

		if (!sectionName.empty()) {
			ImGui::EndGroupPanel();
		}
	}

	auto CheatManagerBase::DrawProfileGlobalActivities() -> void {
		if (ImGui::Button("添加新的配置文件"))
		{
			const std::unordered_set<std::string> profileNameSet = { config::GetProfiles().begin(), config::GetProfiles().end() };
			size_t index = 0;
			std::string name {};
			do 
			{
				index++;
				std::string newName = fmt::format("{} #{}", "配置文件", index);
				if (!profileNameSet.contains(newName)) {
					name = newName;
				}
			} while (name.empty());

			config::CreateProfile(name, false);
		}
	}

	auto CheatManagerBase::DrawProfileEntryActivities(const std::string& profileName) -> void {
		const bool isPopupOpen = ImGui::IsRenamePopupOpened();

		if (isPopupOpen) {
			ImGui::BeginDisabled();
		}

		if (ImGui::SmallButton("重命名")) {
			ImGui::OpenRenamePopup(profileName);
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(reinterpret_cast<const char*>("重命名配置文件"));
		}

		if (isPopupOpen) {
			ImGui::EndDisabled();
		}

		std::string nameBuffer;
		if (ImGui::DrawRenamePopup(nameBuffer))
		{
			config::RenameProfile(profileName, nameBuffer);
		}

		ImGui::SameLine();

		if (ImGui::SmallButton("删除")) {
			config::RemoveProfile(profileName);
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(reinterpret_cast<const char*>("删除配置文件"));
		}

		ImGui::SameLine();

		if (ImGui::SmallButton("复制")) {
			config::DuplicateProfile(profileName);
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(reinterpret_cast<const char*>("复制配置文件"));
		}
	}

	auto CheatManagerBase::DrawProfileEntry(const std::string& profileName) -> void {
		ImGui::Text(profileName.c_str());
	}

	auto CheatManagerBase::DrawProfileTableHeader() -> void {
		ImGui::TableSetupColumn("名称");
	}

	auto CheatManagerBase::GetProfileTableColumnCount() -> int {
		return 1;
	}

	auto CheatManagerBase::DrawProfileConfiguration() -> void {
		static ImGuiTableFlags flags =
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
			| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_ScrollY;
		if (ImGui::BeginTable("ProfileTable", GetProfileTableColumnCount() + 1, flags,
			ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 10), 0.0f))
		{
			DrawProfileTableHeader();
			ImGui::TableSetupColumn("操作");
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableHeadersRow();

			// Copy profiles names
			const auto profiles = config::GetProfiles();
			for (auto& profile : profiles)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGui::PushID(profile.c_str());
				DrawProfileEntry(profile);
				ImGui::TableNextColumn();

				DrawProfileEntryActivities(profile);
				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		DrawProfileGlobalActivities();
	}

	auto CheatManagerBase::DrawProfileLine() -> void {
		if (m_IsProfileConfigurationShowed) {
			ImGui::BeginDisabled();
		}

		const bool buttonPressed = ImGui::Button("配置...");

		if (m_IsProfileConfigurationShowed) {
			ImGui::EndDisabled();
		}

		if (buttonPressed) {
			m_IsProfileConfigurationShowed = !m_IsProfileConfigurationShowed;
		}

		ImGui::SameLine();

		auto& profiles = config::GetProfiles();
		auto& currentProfile = config::CurrentProfileName();

		constexpr float width = 200.0f;
		ImGui::SetNextItemWidth(width);
		if (ImGui::BeginCombo("配置", currentProfile.c_str()))
		{
			for (auto& name : profiles)
			{
				const bool is_selected = (currentProfile == name);
				if (ImGui::Selectable(name.c_str(), is_selected)) {
					config::ChangeProfile(name);
				}

				if (ImGui::IsItemHovered() && CalcWidth(name) > width) {
					ShowHelpText(name.c_str());
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	auto CheatManagerBase::DrawStatus() const -> void {
		// Drawing status window
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

		const auto& settings = feature::Settings::GetInstance();
		if (!settings->f_StatusMove) {
			flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
		}

		ImGui::Begin("作弊状态", nullptr, flags);

		static ImGuiTableFlags tabFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

		if (ImGui::BeginTable("启用列表", 1, tabFlags))
		{
			ImGui::TableSetupColumn("已启用功能");
			ImGui::TableHeadersRow();

			int row = 0;

			for (auto& feature : m_Features)
			{
				if (feature->NeedStatusDraw())
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					feature->DrawStatus();

					const ImU32 row_bg_color = ImGui::GetColorU32(
					ImVec4(0.2f + row * 0.1f, 0.1f + row * 0.05f, 0.1f + row * 0.03f, 0.85f));
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);
					row++;
				}
			}
			ImGui::EndTable();
		}

		ImGui::End();
	}

	auto CheatManagerBase::DrawInfo() -> void {
		const auto& settings = feature::Settings::GetInstance();

		// Drawing status window
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

		if (!settings->f_InfoMove) {
			flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
		}

		auto checkLambda = [](const Feature* feat) { return feat->NeedInfoDraw(); };
		const bool showAny = std::ranges::any_of(m_Features, checkLambda);
		if (!showAny && !settings->f_StatusMove) {
			return;
		}

		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.04f, 0.05f, 0.05f, 0.90f));
		ImGui::Begin("信息窗口", nullptr, flags);
		//ImGui::PopStyleColor();

		if (!showAny)
		{
			ImGui::Text(reinterpret_cast<const char*>("无内容"));
			ImGui::End();
			return;
		}

		for (auto& moduleName : m_ModuleOrder)
		{
			auto& sections = m_FeatureMap[moduleName];
			const bool moduleShowAny = std::ranges::any_of(sections,
			                                               [](const auto& iter)
			                                               {
				                                               return std::ranges::any_of(iter.second.begin(), iter.second.end(),
				                                                                  [](const auto feat)
				                                                                  {
					                                                                  return feat->NeedInfoDraw();
				                                                                  });
			                                               }
			);
			if (!moduleShowAny) {
				continue;
			}

			ImGui::BeginGroupPanel(moduleName.c_str());

			for (auto& features : sections | std::views::values)
			{
				for (auto& feature : features)
				{
					if (!feature->NeedInfoDraw()) {
						continue;
					}

					ImGui::PushID(&feature);
					feature->DrawInfo();
					ImGui::PopID();
				}
			}

			ImGui::EndGroupPanel();
		}

		ImGui::End();
	}

	auto CheatManagerBase::DrawFps() -> void {
		const auto& settings = feature::Settings::GetInstance();

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

		if (!settings->f_FpsMove) {
			flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove;
		}

		if (ImGui::Begin("FPS", nullptr, flags))
		{
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}

	auto CheatManagerBase::DrawNotifications() -> void {
		ImGui::RenderNotifications();
	}

	
	auto CheatManagerBase::OnRender() -> void {
		const auto settings = feature::Settings::GetInstance();

		const auto bg = ImGui::GetBackgroundDrawList();
		DrawTextWithOutline(bg, { 150, 50 }, "本程序完全免费禁止倒卖", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });
		DrawTextWithOutline(bg, { 150, 66 }, "本程序只在官方QQ群发布", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });
		DrawTextWithOutline(bg, { 150, 82 }, "其他渠道均为第三方搬运", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });
		DrawTextWithOutline(bg, { 150, 98 }, "搬运渠道不保证程序无毒", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });
		DrawTextWithOutline(bg, { 150, 114 }, "官方QQ群: 472659840", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });
		DrawTextWithOutline(bg, { 150, 130 }, "加入官方群更快获取更新", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });
		DrawTextWithOutline(bg, { 150, 146 }, "联系我请访问 遂沫.com", ImColor{ 203, 130, 190 }, 1, OutlineSide::All, ImColor{ 0,0,0 });

		if (s_IsMenuShowed) {
			DrawMenu();
		}

		if (m_IsProfileConfigurationShowed)
		{
			ImGui::SetNextWindowSize({ 0, ImGui::GetTextLineHeightWithSpacing() * 11 }, ImGuiCond_FirstUseEver);
			if (ImGui::Begin("配置描述文件配置", &m_IsProfileConfigurationShowed)) {
				DrawProfileConfiguration();
				ImGui::End();
			}
		}

		if (settings->f_StatusShow) {
			DrawStatus();
		}

		if (settings->f_InfoShow) {
			DrawInfo();
		}

		if (settings->f_FpsShow) {
			DrawFps();
		}

		if (settings->f_NotificationsShow) {
			DrawNotifications();
		}

		if (settings->f_ShowStyleEditor) {
			ImGui::ShowStyleEditor();
		}

		if (settings->f_MenuKey.value().IsReleased() && !ImGui::IsAnyItemActive()) {
			ToggleMenuShow();
		}

		DrawExternal();
	}

	auto CheatManagerBase::CheckToggles(short key) -> void {
		if (s_IsMenuShowed || renderer::IsInputLocked()) {
			return;
		}

		const auto& settings = feature::Settings::GetInstance();
		if (!settings->f_HotkeysEnabled) {
			return;
		}

		for (auto& field : config::GetFields<config::Toggle<Hotkey>>())
		{
			auto& toggle = field.value();
			if (toggle.value.IsPressed(key))
			{
				toggle.enabled = !toggle.enabled;
				field.FireChanged();

				std::string title = fmt::format("{}: {}", field.friendName(), toggle ? "启用" : "禁用");
				ImGuiToast toast(ImGuiToastType_None, settings->f_NotificationsDelay);
				toast.set_title(title.c_str());
				ImGui::InsertNotification(toast);
			}
		}
	}

	bool menuToggled = false;

	auto CheatManagerBase::ToggleMenuShow() -> void {
		s_IsMenuShowed = !s_IsMenuShowed;
		renderer::SetInputLock(this, s_IsMenuShowed && m_IsBlockingInput);
		menuToggled = true;
	}

	auto CheatManagerBase::OnKeyUp(short key, bool& cancelled) -> void {
		const auto& settings = feature::Settings::GetInstance();
		if (!settings->f_MenuKey.value().IsPressed(key))
		{
			CheckToggles(key);
		}
	}

	auto CheatManagerBase::OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& canceled) -> void {
		if (!menuToggled) {
			return;
		}

		menuToggled = false;

		if (s_IsMenuShowed)
		{
			m_IsPrevCursorActive = CursorGetVisibility();
			if (!m_IsPrevCursorActive) {
				CursorSetVisibility(true);
			}
		}
		else if (!m_IsPrevCursorActive) {
			CursorSetVisibility(false);
		}
	}

	auto CheatManagerBase::IsMenuShowed() -> bool {
		return s_IsMenuShowed;
	}

	auto CheatManagerBase::PushFeature(Feature* feature) -> void {
		m_Features.push_back(feature);

		auto& info = feature->GetGUIInfo();
		if (!m_FeatureMap.contains(info.moduleName))
		{
			m_FeatureMap[info.moduleName] = {};
			m_ModuleOrder.push_back(info.moduleName);
		}

		auto& sectionMap = m_FeatureMap[info.moduleName];
		const std::string sectionName = info.isGroup ? info.name : std::string();
		if (!sectionMap.contains(sectionName)) {
			sectionMap[sectionName] = {};
		}

		auto& featureList = sectionMap[sectionName];
		featureList.push_back(feature);
	}

	auto CheatManagerBase::AddFeature(Feature* feature) -> void {
		PushFeature(feature);
	}

	auto CheatManagerBase::AddFeatures(const std::vector<Feature*>& features) -> void {
		for (const auto& feature : features)
		{
			PushFeature(feature);
		}
	}

	auto CheatManagerBase::SetModuleOrder(const std::vector<std::string>& moduleOrder) -> void {
		std::unordered_set<std::string> moduleSet;
		moduleSet.insert(m_ModuleOrder.begin(), m_ModuleOrder.end());

		m_ModuleOrder.clear();

		for (auto& moduleName : moduleOrder)
		{
			if (!m_FeatureMap.contains(moduleName)) {
				continue;
			}

			m_ModuleOrder.push_back(moduleName);
			moduleSet.erase(moduleName);
		}

		for (auto& moduleName : moduleSet)
		{
			m_ModuleOrder.push_back(moduleName);
		}
	}
}
