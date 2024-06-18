#include "PickitemMemory.h"

#include "../../../CLASS/Role.h"

cheat::feature::PickitemMemory* cheat::feature::PickitemMemory::GetInstance() {
	static PickitemMemory role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::PickitemMemory::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "内存", "物品", true };
	return info;
}
void cheat::feature::PickitemMemory::DrawMain() {
	ConfigWidget(f_AutoFrie, "全枪全自动");
}
bool cheat::feature::PickitemMemory::NeedStatusDraw() const { return f_AutoFrie; }
void cheat::feature::PickitemMemory::DrawStatus() {
	if (f_AutoFrie) {
		ImGui::Text("全枪自动");
	}
}
bool cheat::feature::PickitemMemory::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::PickitemMemory::DrawInfo() { Feature::DrawInfo(); }
void cheat::feature::PickitemMemory::DrawExternal() { Feature::DrawExternal(); }

void cheat::feature::PickitemMemory::Update() {
	try {
		if (f_AutoFrie && !IsBadReadPtr(Role::localRole, sizeof(void*)) && RoleLogic::UserWeapon.Get(Role::roleLogic.Get(Role::localRole)) && !IsBadReadPtr(RoleLogic::UserWeapon.Get(Role::roleLogic.Get(Role::localRole)), sizeof(void*))) {
			SOWeaponControl::HasAutoFire.Set(PickItemNet::MySOWeaponControl.Get(RoleLogic::UserWeapon.Get(Role::roleLogic.Get(Role::localRole))), f_AutoFrie);
		}
	}
	catch (...) { LOG_ERROR("error"); }
}

cheat::feature::PickitemMemory::PickitemMemory() :
NF(f_AutoFrie, "全枪全自动", "Pickitem::Memory", false) {
	Main::GameUpdateEvent += MY_METHOD_HANDLER(cheat::feature::PickitemMemory::Update);
}
