#include "Analyze.h"
const Feature::GuiInfo& Analyze::GetInfo() const { return *new GuiInfo{ "调试", "分析", true, false, false }; };
void Analyze::Draw() {
}
void Analyze::Render() {
	if (ImGui::BeginTable("List", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("步骤", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("用时 (微秒)", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		float allTime;
		for (const auto& [name, value] : time) {
			allTime += value;
			ImGui::TableNextRow();
			if (ImGui::TableSetColumnIndex(0)) ImGui::Text((name).c_str());
			if (ImGui::TableSetColumnIndex(1)) ImGui::Text(("%0.6f"), value);
		}

		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text("allTime");
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text(("%0.6f ms"), allTime / 1000);

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
