#include "ItemEsp.h"
#include "../../../GameDefine/PickItem/PickItem.h"

auto ItemEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>("物品"), reinterpret_cast<const char*>("透视"), true, false, true }; }
void ItemEsp::Draw() {
	if (enable) {
		const auto bg = ImGui::GetBackgroundDrawList();
		std::lock_guard lock(PickItem::mutex);
		for (const auto& obj : PickItem::vector) {
			if (obj && obj->net && obj->config) {
				try {
					if (auto sPoint = II::Camera::GetAllCamera()[0]->WorldToScreenPoint(obj->net->syncPoint, II::Camera::Eye::Mono);
						sPoint.z > -1 && (sPoint.y < static_cast<float>(windowHeight) && sPoint.y > 0) && (sPoint.z < static_cast<float>(windowWidth) && sPoint.z > 0)) {
						sPoint.y = static_cast<float>(windowHeight) - sPoint.y;
						DrawTextWithOutline(bg, { sPoint.x, sPoint.y }, std::format("{}", obj->config->itemName).c_str(), ImColor{ 255,255,0 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
					}
				}
				catch (...) {
					ERROR()
				}
			}
		}
	}
}
void ItemEsp::Render() {
	ImGui::Checkbox("启用", &enable);
	if (enable && ImGui::BeginTable("Esp", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 4))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"启用"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("名称ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.name);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("名称");
		ImGui::PopID();

		ImGui::PushID("数量ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.num);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("数量");
		ImGui::PopID();

		ImGui::EndTable();
	}
}
void ItemEsp::Update() { Feature::Update(); }
void ItemEsp::Save(nlohmann::json& json) { Feature::Save(json); }
void ItemEsp::Load(nlohmann::json& json) { Feature::Load(json); }
auto ItemEsp::GetInstance() -> ItemEsp* {
	static ItemEsp ItemEsp;
	return &ItemEsp;
}
