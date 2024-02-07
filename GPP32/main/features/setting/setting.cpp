#include "setting.h"
const Feature::GuiInfo& Setting::GetInfo() const { return *new GuiInfo{ "设置", "性能", true, false, false }; };
void Setting::Draw() { Feature::Draw(); }
void Setting::Render() {
	ImGui::Text("数据采样率");
	ImGui::SameLine();
	ImGui::RadioButton("30帧", &upDateSpeed, 32);
	ImGui::SameLine();
	ImGui::RadioButton("45帧", &upDateSpeed, 22);
	ImGui::SameLine();
	ImGui::RadioButton("60帧", &upDateSpeed, 16);
}
void Setting::Update() { Feature::Update(); }
void Setting::Save(nlohmann::json& json) { Feature::Save(json); }
void Setting::Load(nlohmann::json& json) { Feature::Load(json); }
auto Setting::GetInstance() -> Setting* { 
	static Setting setting;
	return &setting;
}
