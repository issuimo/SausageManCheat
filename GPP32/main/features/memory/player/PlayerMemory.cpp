#include "PlayerMemory.h"
#include "../../../GameDefine/Role/Role.h"

const Feature::GuiInfo& PlayerMemory::GetInfo() const { return *new GuiInfo{ (const char*)u8"内存", (const char*)u8"玩家", true, true, false }; };
void PlayerMemory::Draw() { Feature::Draw(); }
void PlayerMemory::Render() {
	if (ImGui::BeginTabBar("memList")) {

		ImGui::EndTabBar();
	}
}
void PlayerMemory::Update() {
	
}
void PlayerMemory::Save(nlohmann::json& json) {

}
void PlayerMemory::Load(nlohmann::json& json) {
	if (json.contains("PlayerMemory")) {

	}
}
auto PlayerMemory::GetInstance() -> PlayerMemory* {
	static PlayerMemory PlayerMemory;
	return &PlayerMemory;
}
