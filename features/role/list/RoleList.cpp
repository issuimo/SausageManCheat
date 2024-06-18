#include "RoleList.h"

#include "../../../CLASS/Role.h"

cheat::feature::RoleList* cheat::feature::RoleList::GetInstance() {
	static RoleList role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::RoleList::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "列表", "玩家", true };
	return info;
}
void cheat::feature::RoleList::DrawMain() {
	ImGui::Text(std::format("我的地址: {}", static_cast<void*>(Role::localRole)).c_str());
	ImGui::Text(std::format("{} 名玩家", Role::vector.GetValue().size()).c_str());
	if (ImGui::BeginTable("PlayerList", 3,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("血量", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& player : Role::vector.GetValue()) {
			ImGui::TableNextRow();
			ImGui::PushID(&player);
			try {
				if (!IsBadReadPtr(player, sizeof(void*)) && !IsBadReadPtr(Role::roleLogic.Get(player), sizeof(void*))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(RoleLogic::NickName.Get(Role::roleLogic.Get(player))->ToString().c_str());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text(std::format("{}", RoleLogic::hp.Get(Role::roleLogic.Get(player))).c_str());
					if (ImGui::TableSetColumnIndex(2)) ImGui::Text(std::format("{}", static_cast<void*>(player)).c_str());
				}
			} catch (const SEH_exception& e) {
				LOG_ERROR("%s", e.what());
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
bool cheat::feature::RoleList::NeedStatusDraw() const { return Feature::NeedStatusDraw(); }
void cheat::feature::RoleList::DrawStatus() { Feature::DrawStatus(); }
bool cheat::feature::RoleList::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::RoleList::DrawInfo() { Feature::DrawInfo(); }
void cheat::feature::RoleList::DrawExternal() { Feature::DrawExternal(); }
cheat::feature::RoleList::RoleList() {  }
