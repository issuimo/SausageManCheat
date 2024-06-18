#include "CarEsp.h"
#include "../../../CLASS/Car.h"
#include "../../../CLASS/Role.h"

#include "../list/CarList.h"

cheat::feature::CarEsp* cheat::feature::CarEsp::GetInstance() { 
	static CarEsp role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::CarEsp::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "透视", "载具", true };
	return info;
}
void cheat::feature::CarEsp::DrawMain() {
	ConfigWidget(f_Enable);
}
bool cheat::feature::CarEsp::NeedStatusDraw() const { return f_Enable; }
void cheat::feature::CarEsp::DrawStatus() { ImGui::Text("载具透视"); }
bool cheat::feature::CarEsp::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::CarEsp::DrawInfo() { Feature::DrawInfo(); }
void cheat::feature::CarEsp::DrawExternal() {
	if (f_Enable) {
		const auto bg = ImGui::GetBackgroundDrawList();

		II::Vector3 mPoint{};

		II::Camera* cam;

		try {
			if (const auto cams = II::Camera::GetAllCamera(); !cams.empty()) cam = cams[0];
		} catch (...) {
			LOG_ERROR("error");
			return;
		}

		try {
			if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(void*))) mPoint = RoleLogic::NowPoint.Get(Role::roleLogic.Get(Role::localRole));
		}
		catch (...) { LOG_ERROR("error"); return;; }

		for (const auto car : Car::vector.GetValue()) {
			try {
				if (!IsBadReadPtr(car, sizeof(void*))) {
					const auto nPoint = Car::myTransform.Get(car)->GetPosition();
					auto sPoint = cam->WorldToScreenPoint(nPoint);
					if (sPoint.z < 0) { continue; }
					sPoint.y = renderer::windowHeight - sPoint.y;
					DrawTextWithOutline(bg, { sPoint.x, sPoint.y }, std::format("{}m | {}", (int)sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2)), CarList::name[Car::CarDataId.Get(car)->ToString()]).c_str(), { 255, 255, 0 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
				}
			}
			catch (...) { LOG_ERROR("error"); return;; }

		}
	}
}
cheat::feature::CarEsp::CarEsp() : NF(f_Enable, "透视", "Car::Esp", false) { }
