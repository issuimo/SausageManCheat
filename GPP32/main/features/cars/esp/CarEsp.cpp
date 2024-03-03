#include "CarEsp.h"
#include "../../../GameDefine/Car/Car.h"
#include "../../../GameDefine/Role/Role.h"

#include "../../setting/analyze.h"
#include "../list/CarList.h"

auto CarEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ (const char*)u8"载具", (const char*)u8"透视", true, false, true }; }
void CarEsp::Draw() {
	if (enable) {
		std::lock_guard lock(Car::mutex);
		std::lock_guard lock2(Role::mutex);
		const auto bg = ImGui::GetBackgroundDrawList();
		TIME_LOCK;
		try {
			[&]() {
				__try {
					[&]() {
						II::Vector3 mPoint{}; {
							if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(RoleLogic))) {
								mPoint = Role::localRole->roleLogic->NowPoint;
							}
						}
						for (auto car : Car::vector) {
							if (!IsBadReadPtr(car, sizeof(Car))) {
								auto sPoint = WorldToScreenPoint(car->myTransform->GetPosition());
								if (sPoint.z < 0) { continue; }
								auto nPoint = car->myTransform->GetPosition();
								DrawTextWithOutline(bg, DrawHelp::BoxScreen::ToVec2(sPoint), std::format("{}m | {}", (int)sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2)), CarList::nameZh[car->CarDataId->ToString()]).c_str(), {255, 255, 0}, 1, DrawHelp::OutlineSide::All, ImColor{0,0,0});
							}
						}
					}();
				} __except (EXCEPTION_EXECUTE_HANDLER) {
					[]() {
						ERROR("CarEsp (except)");
					}();
				}
			}();
			
		} catch (...) {
			ERROR("CarEsp->nullptr")
		}
	}
}
void CarEsp::Render() {
	ImGui::Checkbox((const char*)u8"启用", &enable);
}
void CarEsp::Update() { Feature::Update(); }
void CarEsp::Save(nlohmann::json& json) { 
	json["CarEsp"]["enable"] = enable;
}
void CarEsp::Load(nlohmann::json& json) { 
	if (json.contains("CarEsp")) {
		if (json["CarEsp"].contains("enable")) {
			enable = json["CarEsp"]["enable"];
		}
	}
}
auto CarEsp::GetInstance() -> CarEsp* {
	static CarEsp CarEsp;
	return &CarEsp;
}
