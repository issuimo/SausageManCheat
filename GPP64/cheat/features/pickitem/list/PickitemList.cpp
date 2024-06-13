#include "PickitemList.h"

#include "../../../CLASS/PickItem.h"

cheat::feature::PickitemList* cheat::feature::PickitemList::GetInstance() {
	static PickitemList role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::PickitemList::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "列表", "物品", true };
	return info;
}
void cheat::feature::PickitemList::DrawMain() {
	ImGui::Text(std::format("{} 个物品", PickItem::vector.GetValue().size()).c_str());
	if (ImGui::BeginTable("PickitemList", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& obj : PickItem::vector.GetValue()) {
			ImGui::TableNextRow();
			ImGui::PushID(&obj);
			try {
				if (!IsBadReadPtr(obj, sizeof(void*)) && !IsBadReadPtr(PickItem::pickItemData.Get(obj), sizeof(void*))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(PickItem::MyPickItemNet.Get(obj)))->ToString().data());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text("%p", static_cast<void*>(obj));
				}
			} catch (const SEH_exception& e) {
				LOG_ERROR("%s", e.what());
			} catch (...) {
				LOG_ERROR("error");
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
bool cheat::feature::PickitemList::NeedStatusDraw() const { return Feature::NeedStatusDraw(); }
void cheat::feature::PickitemList::DrawStatus() { Feature::DrawStatus(); }
bool cheat::feature::PickitemList::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::PickitemList::DrawInfo() { Feature::DrawInfo(); }
void cheat::feature::PickitemList::DrawExternal() { Feature::DrawExternal(); }
cheat::feature::PickitemList::PickitemList() {  }
