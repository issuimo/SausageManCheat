#include "ItemEsp.h"
#include "../../../GameDefine/PickItem/PickItem.h"
#include "../../../GameDefine/Role/Role.h"

auto ItemEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>(u8"物品"), reinterpret_cast<const char*>(u8"透视"), true, false, true }; }
void ItemEsp::Draw() {
	if (enable) {
		const auto bg = ImGui::GetBackgroundDrawList();
		std::lock_guard lock(PickItem::mutex);
		try {
			II::Vector3 mPoint{}; {
				std::lock_guard lock2(Role::mutex);
				if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(RoleLogic))) {
					mPoint = Role::localRole->roleLogic->NowPoint;
				}
			}
			auto ca = II::Camera::GetAllCamera()[0];
			for (const auto& obj : PickItem::vector) {
				if (!IsBadReadPtr(obj, sizeof(PickItem)) && !IsBadReadPtr(obj->pickItemData, sizeof(PickItemDataConfig)) && !IsBadReadPtr(obj->MyPickItemNet, sizeof(PickItemNet))) {
					auto sPoint = ca->WorldToScreenPoint(obj->MyPickItemNet->SyncPoint, II::Camera::Eye::Mono);
					if (sPoint.z < 0) { continue; }
					auto nPoint = obj->MyPickItemNet->SyncPoint;
					sPoint.y = static_cast<float>(windowHeight) - sPoint.y;
					if (obj->pickItemData->itemName->ToString().find(GbkToUtf8("灸灸")) != std::string::npos) {
						DrawTextWithOutline(bg, { sPoint.x, sPoint.y }, std::format("{}m | {}", (int)sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2)), obj->pickItemData->itemName->ToString()).c_str(), ImColor{ 255,0,0 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
					} else {
						DrawTextWithOutline(bg, { sPoint.x, sPoint.y }, std::format("{}m | {}", (int)sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2)), obj->pickItemData->itemName->ToString()).c_str(), ImColor{ 255,255,0 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
					}
					
				}
			}
		} catch (...) {
			ERROR("ItemEsp->nullptr")
		}
	}
}
void ItemEsp::Render() {
	ImGui::Checkbox((const char*)u8"启用", &enable);
}
void ItemEsp::Update() { Feature::Update(); }
void ItemEsp::Save(nlohmann::json& json) { 
	json["ItemESP"]["enable"] = enable;
}
void ItemEsp::Load(nlohmann::json& json) { 
	if (json.contains("ItemESP")) {
		if (json["ItemESP"].contains("enable")) {
			enable = json["ItemESP"]["enable"];
		}
	}
}
auto ItemEsp::GetInstance() -> ItemEsp* {
	static ItemEsp ItemEsp;
	return &ItemEsp;
}
