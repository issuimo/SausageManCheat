#include "PlayerMemory.h"
#include "../../../GameDefine/Role/Role.h"

const Feature::GuiInfo& PlayerMemory::GetInfo() const { return *new GuiInfo{ (const char*)u8"内存", (const char*)u8"玩家", true, true, false }; };
void PlayerMemory::Draw() { Feature::Draw(); }
void PlayerMemory::Render() {
	if (ImGui::BeginTabBar("memList")) {
		if (ImGui::BeginTabItem((const char*)u8"缩放")) {
			if (ImGui::Button((const char*)u8"重置")) {
				scale.v = 1;
			}
			ImGui::SameLine();
			ImGui::Checkbox((const char*)u8"启用", &scale.enable);
			if (scale.enable) {
				ImGui::SliderFloat((const char*)u8"倍", &scale.v, 0.1, 2.5);
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}
void PlayerMemory::Update() {
	if (scale.enable) {
		std::lock_guard lock(Role::mutex);
		try {
			for (const auto& player : Role::vector) {
				if (player == Role::localRole ||
					IsBadReadPtr(Role::localRole, sizeof(RoleLogic)) ||
					IsBadReadPtr(player, sizeof(Role)) ||
					IsBadReadPtr(player->MyRoleControl, sizeof(RoleControl)) ||
					IsBadReadPtr(player->roleLogic, sizeof(RoleLogic)) ||
					Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) continue;
				player->roleLogic->RoleSize = scale.v;
			}
		} catch (...) {
			ERROR("PlayerMemory->scale")
		}
	}
}
void PlayerMemory::Save(nlohmann::json& json) {
	json["PlayerMemory"]["scale_enable"] = scale.enable;
	json["PlayerMemory"]["scale_v"] = scale.v;
}
void PlayerMemory::Load(nlohmann::json& json) {
	if (json.contains("PlayerMemory")) {
		if (json["PlayerMemory"].contains("scale_enable")) {
			scale.enable = json["PlayerMemory"]["scale_enable"];
		}
		if (json["PlayerMemory"].contains("scale_v")) {
			scale.v = json["PlayerMemory"]["scale_v"];
		}
	}
}
auto PlayerMemory::GetInstance() -> PlayerMemory* {
	static PlayerMemory PlayerMemory;
	return &PlayerMemory;
}
