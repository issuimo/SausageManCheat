#include "setting.h"
const Feature::GuiInfo& Setting::GetInfo() const { return *new GuiInfo{ (const char*)u8"设置", (const char*)u8"性能", true, false, false }; };
void Setting::Draw() { Feature::Draw(); }
void Setting::Render() {
	ImGui::Text((const char*)u8"数据采样率");
	ImGui::SameLine();
	ImGui::RadioButton((const char*)u8"10帧", &upDateSpeed, 100);
	ImGui::SameLine();
	ImGui::RadioButton((const char*)u8"20帧", &upDateSpeed, 50);
	ImGui::SameLine();
	ImGui::RadioButton((const char*)u8"30帧", &upDateSpeed, 32);
}
void Setting::Update() { Feature::Update(); }
void Setting::Save(nlohmann::json& json) { Feature::Save(json); }
void Setting::Load(nlohmann::json& json) { Feature::Load(json); }
auto Setting::GetInstance() -> Setting* { 
	static Setting setting;
	return &setting;
}
