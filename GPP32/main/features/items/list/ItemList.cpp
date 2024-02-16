#include "ItemList.h"
#include "../../../GameDefine/PickItem/PickItem.h"

auto ItemList::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>("物品"), reinterpret_cast<const char*>("列表"), true, false, false }; }
void ItemList::Draw() { Feature::Draw(); }
void ItemList::Render() {
	std::lock_guard lock(PickItem::mutex);
	ImGui::Text(std::format("{} 个物品", PickItem::vector.size()).c_str());
	if (ImGui::BeginTable("List", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"ID"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& obj : PickItem::vector) {
			ImGui::PushID(obj);
			try {
				ImGui::TableNextRow();
				if (obj->config && obj->net) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(std::format("{}", obj->config->id).c_str());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text(std::format("{}", obj->config->itemName).c_str());
				}
			} catch (...) {}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
void ItemList::Update() { Feature::Update(); }
void ItemList::Save(nlohmann::json& json) { Feature::Save(json); }
void ItemList::Load(nlohmann::json& json) { Feature::Load(json); }
auto ItemList::GetInstance() -> ItemList* {
	static ItemList itemList;
	return &itemList;
}
