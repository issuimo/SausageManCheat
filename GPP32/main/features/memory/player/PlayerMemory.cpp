#include "PlayerMemory.h"
#include "../../../GameDefine/Role/Role.h"

const Feature::GuiInfo& PlayerMemory::GetInfo() const { return *new GuiInfo{ (const char*)u8"内存", (const char*)u8"玩家", true, true, false }; };
void PlayerMemory::Draw() { Feature::Draw(); }
void PlayerMemory::Render() {
	if (ImGui::BeginTabBar("memList")) {
		if (ImGui::BeginTabItem((const char*)u8"缩放")) {
			ImGui::TextColored(ImColor{ 255,0,0 }, (const char*)u8"有较大概率封号");
			if (ImGui::Button((const char*)u8"重置")) {
				scale.v = 1;
			}
			ImGui::SameLine();
			ImGui::Checkbox(reinterpret_cast<const char*>(u8"启用"), &scale.enable);
			if (scale.enable) {
				ImGui::SliderFloat((const char*)u8"倍", &scale.v, 0.1, 2.5);
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem((const char*)u8"移动")) {
			ImGui::Checkbox((const char*)u8"天外飞仙", &enable);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}
void PlayerMemory::Update() {
	std::lock_guard lock(Role::mutex);

	if (scale.enable) {
		[&]() {
			__try {
				[&]() {
					for (const auto& player : Role::vector) {
						if (player == Role::localRole ||
							IsBadReadPtr(Role::localRole, sizeof(RoleLogic)) ||
							IsBadReadPtr(player, sizeof(Role)) ||
							IsBadReadPtr(player->MyRoleControl, sizeof(RoleControl)) ||
							IsBadReadPtr(player->roleLogic, sizeof(RoleLogic)) ||
							Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) continue;
						player->roleLogic->RoleSize = scale.v;
					}

				}();
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				[]() {
					ERROR("PlayerMemory Error (except)");
				}();
			}
		}();
	}
	

	if (enable) {

		// W
		if (GetKeyState(0x57) <= -127) {
			[&]() {
				__try {
					[&]() {
						if (Role::localRole && Role::localRole->_myGameObj) {
							auto point = Role::localRole->_myGameObj->GetTransform()->GetPosition();
							point.x += 2;
							Role::localRole->_myGameObj->GetTransform()->SetPosition(point);
						}
					}();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("PlayerMemory Error (except)");
					}();
				}
			}();
		}
		// S
		if (GetKeyState(0x53) <= -127) {
			[&]() {
				__try {
					[&]() {
						if (Role::localRole && Role::localRole->_myGameObj) {
							auto point = Role::localRole->_myGameObj->GetTransform()->GetPosition();
							point.x -= 2;
							Role::localRole->_myGameObj->GetTransform()->SetPosition(point);
						}	
					}();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("PlayerMemory Error (except)");
					}();
				}
			}();
		}
		// A
		if (GetKeyState(0x41) <= -127) {
			[&]() {
				__try {
					[&]() {
						if (Role::localRole && Role::localRole->_myGameObj) {
							auto point = Role::localRole->_myGameObj->GetTransform()->GetPosition();
							point.z += 2;
							Role::localRole->_myGameObj->GetTransform()->SetPosition(point);
						}
					}();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("PlayerMemory Error (except)");
					}();
				}
			}();
		}
		// D
		if (GetKeyState(0x44) <= -127) {
			[&]() {
				__try {
					[&]() {
						if (Role::localRole && Role::localRole->_myGameObj) {
							auto point = Role::localRole->_myGameObj->GetTransform()->GetPosition();
							point.z -= 2;
							Role::localRole->_myGameObj->GetTransform()->SetPosition(point);
						}
					}();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("PlayerMemory Error (except)");
					}();
				}
			}();
		}
		// SPACE
		if (GetKeyState(VK_SPACE) <= -127) {
			[&]() {
				__try {
					[&]() {
						if (Role::localRole && Role::localRole->_myGameObj) {
							auto point = Role::localRole->_myGameObj->GetTransform()->GetPosition();
							point.y += 2;
							Role::localRole->_myGameObj->GetTransform()->SetPosition(point);
						}
					}();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("PlayerMemory Error (except)");
					}();
				}
			}();
		}
		// LCTRL
		if (GetKeyState(VK_LCONTROL) <= -127) {
			[&]() {
				__try {
					[&]() {
						if (Role::localRole && Role::localRole->_myGameObj) {
							auto point = Role::localRole->_myGameObj->GetTransform()->GetPosition();
							point.y -= 2;
							Role::localRole->_myGameObj->GetTransform()->SetPosition(point);
						}
					}();
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("PlayerMemory Error (except)");
					}();
				}
			}();
		}
	}
}
void PlayerMemory::Save(nlohmann::json& json) {
	json["PlayerMemory"]["fly"] = enable;
	json["PlayerMemory"]["scale_enable"] = scale.enable;
	json["PlayerMemory"]["scale_v"] = scale.v;
}
void PlayerMemory::Load(nlohmann::json& json) {
	if (json.contains("PlayerMemory")) {
		if (json["PlayerMemory"].contains("fly")) {
			enable = json["PlayerMemory"]["fly"];
		}
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
