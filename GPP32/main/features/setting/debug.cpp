#include "debug.h"
const Feature::GuiInfo& Debug::GetInfo() const { return *new GuiInfo{ (const char*)u8"调试", (const char*)u8"界面", true, false, true }; };
void Debug::Draw() { 
	if (color) {
		ImGui::ShowDemoWindow();
	}
}
void Debug::Render() {
	ImGui::Checkbox((const char*)u8"打开Demo", &color);
	ImGui::SliderFloat("R", &r, 0, 1);
	ImGui::SliderFloat("G", &g, 0, 1);
	ImGui::SliderFloat("B", &b, 0, 1);
	if (ImGui::Button((const char*)u8"载入")) {
		LoadCol(r, g, b);
	}
}
void Debug::Update() { Feature::Update(); }
void Debug::Save(nlohmann::json& json) {
	json["Debug"]["color_r"] = r;
	json["Debug"]["color_g"] = g;
	json["Debug"]["color_b"] = b;
}
void Debug::Load(nlohmann::json& json) {
	if (json.contains("Debug")) {
		if (json["Debug"].contains("color_r")) {
			r = json["Debug"]["color_r"];
		}
		if (json["Debug"].contains("color_g")) {
			g = json["Debug"]["color_g"];
		}
		if (json["Debug"].contains("color_b")) {
			b = json["Debug"]["color_b"];
		}
	}
}
auto Debug::GetInstance() -> Debug* {
	static Debug Debug;
	return &Debug;
}
