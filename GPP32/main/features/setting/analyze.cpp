#include "Analyze.h"
const Feature::GuiInfo& Analyze::GetInfo() const { return *new GuiInfo{ (const char*)u8"调试", (const char*)u8"分析", true, false, false }; };
void Analyze::Draw() {
}
void Analyze::Render() {
	if (ImGui::BeginTable("List", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn((const char*)u8"步骤", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn((const char*)u8"用时 (微秒)", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		float allTime;
		for (const auto& [name, value] : time) {
			allTime += value;
			ImGui::TableNextRow();
			if (ImGui::TableSetColumnIndex(0)) ImGui::Text(GbkToUtf8(name).c_str());
			if (ImGui::TableSetColumnIndex(1)) ImGui::Text(GbkToUtf8("%0.6f").c_str(), value);
		}

		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text("allTime");
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text(GbkToUtf8("%0.6f ms").c_str(), allTime / 1000);

		ImGui::EndTable();
	}
}
void Analyze::Update() { Feature::Update(); }
void Analyze::Save(nlohmann::json& json) {

}
void Analyze::Load(nlohmann::json& json) {

}
auto Analyze::GetInstance() -> Analyze* {
	static Analyze Analyze;
	return &Analyze;
}
