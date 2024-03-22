#include "PlayerList.h"
#include "../../../GameDefine/Role/Role.h"

auto PlayerList::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ "玩家", "列表", true, false, true }; }
void PlayerList::Draw() {
	const auto bg = ImGui::GetBackgroundDrawList();
	std::lock_guard lock(Role::mutex);
	int fake = 0, num = 0;
	[&] {
		__try {
			[&] {
				for (const auto& player : Role::vector) {
					if (player == Role::localRole ||
						IsBadReadPtr(Role::localRole, sizeof(RoleLogic)) ||
						IsBadReadPtr(player, sizeof(Role)) ||
						IsBadReadPtr(player->roleLogic, sizeof(RoleControl)) ||
						Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) continue;
					if (player->roleLogic->TeamNum == 0) fake++;
					num++;
				}
			}();
		} __except (EXCEPTION_EXECUTE_HANDLER) { [] { ERROR("RoleList->Draw (except)"); }(); }
	}();
	DrawTextWithOutline(bg, { static_cast<float>(windowWidth / 2 - 45), 50.0f }, (std::format("附近人数: {}", num)).c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
	DrawTextWithOutline(bg, { static_cast<float>(windowWidth / 2 - 75), 65.0f }, (std::format("人机: {}", fake)).c_str(), ImColor{ 0, 153, 255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
	DrawTextWithOutline(bg, { static_cast<float>(windowWidth / 2), 65.0f }, (std::format("玩家: {}", num - fake)).c_str(), ImColor{ 255, 63, 48 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
}
void PlayerList::Render() {
	std::lock_guard lock(Role::mutex);
	ImGui::Text((std::format("{} 名玩家", Role::vector.size())).c_str());
	if (ImGui::BeginTable("PlayerList", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("血量", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& player : Role::vector) {
			ImGui::TableNextRow();
			ImGui::PushID(&player);
			try {
				if (!IsBadReadPtr(player, sizeof(Role)) && !IsBadReadPtr(player->roleLogic, sizeof(RoleLogic))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(player->roleLogic->NickName->ToString().c_str());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text(std::format("{}", player->roleLogic->hp).c_str());
				}
			} catch (...) {
				ERROR("PlayerList->nullptr")
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
void PlayerList::Update() { Feature::Update(); }
void PlayerList::Save(nlohmann::json& json) { Feature::Save(json); }
void PlayerList::Load(nlohmann::json& json) { Feature::Load(json); }
auto PlayerList::GetInstance() -> PlayerList* {
	static PlayerList players;
	return &players;
}
