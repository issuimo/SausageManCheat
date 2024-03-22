#include "setting.h"
const Feature::GuiInfo& Setting::GetInfo() const { return *new GuiInfo{ "设置", "性能", true, false, false }; };
void Setting::Draw() { Feature::Draw(); }
void Setting::Render() {
	ImGui::Text("数据采样率");
	ImGui::SameLine();
	ImGui::RadioButton("5帧", &upDateSpeed, 200);
	ImGui::SameLine();
	ImGui::RadioButton("10帧", &upDateSpeed, 100);
	ImGui::SameLine();
	ImGui::RadioButton("20帧", &upDateSpeed, 50);
}
void Setting::Update() { Feature::Update(); }
void Setting::Save(nlohmann::json& json) {
	json["Setting"]["upDateSpeed"] = upDateSpeed;
}
void Setting::Load(nlohmann::json& json) {
	if (json.contains("Setting")) {
		if (json["Setting"].contains("upDateSpeed")) {
			upDateSpeed = json["Setting"]["upDateSpeed"];
		}
	}
}
auto Setting::GetInstance() -> Setting* { 
	static Setting setting;
	return &setting;
}
