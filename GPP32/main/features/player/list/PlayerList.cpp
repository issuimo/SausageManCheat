#include "PlayerList.h"
#include "../../../GameDefine/Role/Role.h"

auto PlayerList::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>(u8"玩家"), reinterpret_cast<const char*>(u8"列表"), true, false, false }; }
void PlayerList::Draw() { Feature::Draw(); }
void PlayerList::Render() {
	std::lock_guard lock(Role::mutex);
	ImGui::Text(GbkToUtf8(std::format("{} 名玩家", Role::vector.size())).c_str());
	if (ImGui::BeginTable("PlayerList", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"血量"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& player : Role::vector) {
			ImGui::TableNextRow();
			ImGui::PushID(&player);
			try {
				if (!IsBadReadPtr(player, sizeof(Role)) && !IsBadReadPtr(player->roleLogic, sizeof(RoleLogic))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(std::format("{}", player->roleLogic->NickName->ToString()).c_str());
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
