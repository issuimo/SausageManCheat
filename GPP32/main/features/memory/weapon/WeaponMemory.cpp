#include "WeaponMemory.h"
#include "../../../GameDefine/Role/Role.h"

const Feature::GuiInfo& WeaponMemory::GetInfo() const { return *new GuiInfo{ "内存", "武器", true, true, false }; };
void WeaponMemory::Draw() { Feature::Draw(); }
void WeaponMemory::Render() {
	if (ImGui::BeginTable("PlayerEsp", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("启用", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("全自动ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &allAuto);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("全自动");
		ImGui::PopID();

		ImGui::EndTable();
	}
}
void WeaponMemory::Update() { 
	try {
		if (Role::localRole && Role::localRole->roleLogic->UserWeapon) {
			if (allAuto) {
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->HasAutoFire = true;
			}
		}
	} catch (...) {
		ERROR("WeaponMemory->Update");
	}
}
void WeaponMemory::Save(nlohmann::json& json) {
	json["WeaponMemory"]["allAuto"] = allAuto;

}
void WeaponMemory::Load(nlohmann::json& json) {
	if (json.contains("WeaponMemory")) {
		if (json["WeaponMemory"].contains("allAuto")) {
			allAuto = json["WeaponMemory"]["allAuto"];
		}
	}
}
auto WeaponMemory::GetInstance() -> WeaponMemory* {
	static WeaponMemory WeaponMemory;
	return &WeaponMemory;
}
