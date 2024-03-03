#include "ItemList.h"
#include "../../../GameDefine/PickItem/PickItem.h"

auto ItemList::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ (const char*)u8"物品", (const char*)u8"列表", true, false, false }; }
void ItemList::Draw() { Feature::Draw(); }
void ItemList::Render() {
	std::lock_guard lock(PickItem::mutex);
	ImGui::Text(GbkToUtf8(std::format("{} 个物品", PickItem::vector.size())).c_str());
	if (ImGui::BeginTable("List", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn((const char*)u8"ID", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& obj : PickItem::vector) {
			ImGui::TableNextRow();
			ImGui::PushID(obj);
			try {
				if (!IsBadReadPtr(obj, sizeof(PickItem)) && !IsBadReadPtr(obj->pickItemData, sizeof(PickItemDataConfig))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(std::format("{}", obj->_autoItemId).c_str());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text(obj->pickItemData->itemName->ToString().c_str());
				}
			} catch (...) {
				ERROR("ItemList->nullptr")
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	/*ImGui::Text(GbkToUtf8(std::format("{} 个物品", PickItem::allVector.size())).c_str());
	if (ImGui::BeginTable("List", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn((const char*)u8"ID", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& obj : PickItem::allVector) {
			ImGui::TableNextRow();
			ImGui::PushID(obj);
			try {
				if (!IsBadReadPtr(obj, sizeof(PickItem)) && !IsBadReadPtr(obj->pickItemData, sizeof(PickItemDataConfig))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(std::format("{}", obj->_autoItemId).c_str());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text(obj->pickItemData->itemName->ToString().c_str());
				}
			}
			catch (...) {
				ERROR("ItemList->nullptr")
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}*/
}
void ItemList::Update() { Feature::Update(); }
void ItemList::Save(nlohmann::json& json) { Feature::Save(json); }
void ItemList::Load(nlohmann::json& json) { Feature::Load(json); }
auto ItemList::GetInstance() -> ItemList* {
	static ItemList itemList;
	return &itemList;
}
