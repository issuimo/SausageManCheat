#include "WeaponMemory.h"
#include "../../../GameDefine/Role/Role.h"

const Feature::GuiInfo& WeaponMemory::GetInfo() const { return *new GuiInfo{ (const char*)u8"�ڴ�", (const char*)u8"����", true, true, false }; };
void WeaponMemory::Draw() { Feature::Draw(); }
void WeaponMemory::Render() {
	if (ImGui::BeginTable("PlayerEsp", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"����"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"����"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("ȫ�Զ�ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &allAuto);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"ȫ�Զ�");
		ImGui::PopID();

		ImGui::PushID("������ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &noRecoil);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"������");
		ImGui::PopID();

		if (noRecoil) {
			ImGui::PushID("����ch");
			ImGui::TableNextRow();
			if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"Y������");
			if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"ֵ", &yRecoil, 0, 8);
			ImGui::PopID();

			ImGui::PushID("����ch");
			ImGui::TableNextRow();
			if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"X������");
			if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"ֵ", &xRecoil, 0, 8);
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
void WeaponMemory::Update() { 
	try {
		if (Role::localRole && Role::localRole->roleLogic->UserWeapon) {
			if (noRecoil) {
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->UpMaxRecoil = yRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->UpMinRecoil = yRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->DownRecoilRatio = yRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->CrouchInfluenceUpRecoilRatio = yRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->ProneInfluenceUpRecoilRatio = yRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->MaxLeftRightRecoil = xRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->ProneInfluenceMaxLeftRightRecoilRatio = xRecoil;
				Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->CrouchInfluenceMaxLeftRightRecoilRatio = xRecoil;

				for (auto ptr : Role::localRole->roleLogic->UserWeapon->MySOWeaponControl->UpRecoilInfo->ToVector()) {
					ptr->UpMaxRecoil = yRecoil;
					ptr->UpMinRecoil = yRecoil;
				}
			}
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
	json["WeaponMemory"]["noRecoil"] = noRecoil;
	json["WeaponMemory"]["yRecoil"] = yRecoil;
	json["WeaponMemory"]["xRecoil"] = xRecoil;
}
void WeaponMemory::Load(nlohmann::json& json) {
	if (json.contains("WeaponMemory")) {
		if (json["WeaponMemory"].contains("allAuto")) {
			allAuto = json["WeaponMemory"]["allAuto"];
		}
		if (json["WeaponMemory"].contains("noRecoil")) {
			noRecoil = json["WeaponMemory"]["noRecoil"];
		}
		if (json["WeaponMemory"].contains("yRecoil")) {
			yRecoil = json["WeaponMemory"]["yRecoil"];
		}
		if (json["WeaponMemory"].contains("xRecoil")) {
			xRecoil = json["WeaponMemory"]["xRecoil"];
		}
	}
}
auto WeaponMemory::GetInstance() -> WeaponMemory* {
	static WeaponMemory WeaponMemory;
	return &WeaponMemory;
}
