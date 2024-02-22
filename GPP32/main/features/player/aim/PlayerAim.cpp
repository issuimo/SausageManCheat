#include "PlayerAim.h"
#include "../../../GameDefine/Role/Role.h"
#include <WinUser.h>
const Feature::GuiInfo& PlayerAim::GetInfo() const { return *new GuiInfo{ (const char*)u8"自瞄", (const char*)u8"玩家", false, true, true }; };
void PlayerAim::Draw() { 
	const auto bg = ImGui::GetBackgroundDrawList();
	if (hasObj) {
		std::lock_guard lock(Role::mutex);
		bg->AddCircleFilled(screenPoint, 5, ImColor{ 255,0,0 }, 15);
	}
	if (enable) {
		bg->AddRect({ (float)(windowWidth / 2 - rangeX), (float)(windowHeight / 2 - rangeY) }, { (float)(windowWidth / 2 + rangeX), (float)(windowHeight / 2 + rangeY) }, ImColor{ 0,255,0 }, 0, 0, 2);
	}
}
void PlayerAim::Render() {
	ImGui::Checkbox((const char*)u8"启用", &enable);
	if (enable && ImGui::BeginTable("PlayerEsp", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"值"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("速度Xch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"水平速度");
		if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"分之一", &xSpeed, 3, 50);
		ImGui::PopID();

		ImGui::PushID("速度Ych");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"垂直速度");
		if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"分之一", &ySpeed, 3, 50);
		ImGui::PopID();

		ImGui::PushID("水平范围ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"水平范围");
		if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"像素", &rangeX, 1, 1200);
		ImGui::PopID();

		ImGui::PushID("垂直范围ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"垂直范围");
		if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"像素", &rangeY, 5, 700);
		ImGui::PopID();

		ImGui::PushID("热键ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"热键");
		if (ImGui::TableSetColumnIndex(1)) {
			if (ImGui::BeginCombo((const char*)u8"热键", keyName.c_str())) {
				if (ImGui::Selectable("Shift")) { aimKey = 16; keyName = "Shift"; }
				if (ImGui::Selectable("RButton")) { aimKey = 2; keyName = "RButton"; }
				if (ImGui::Selectable("LButton")) { aimKey = 1; keyName = "LButton"; }
				if (ImGui::Selectable("Ctrl")) { aimKey = 17; keyName = "Ctrl"; }
				ImGui::EndCombo();
			}
		}
		ImGui::PopID();

		ImGui::EndTable();
	}
}
void PlayerAim::Update() {
	if (enable) {
		std::lock_guard lock(Role::mutex);
		try {
			hasObj = false;
			float len = 99999;
			const auto camera = II::Camera::GetAllCamera()[0];
			for (const auto& player : Role::vector) {
				if (player == Role::localRole ||
					IsBadReadPtr(Role::localRole, sizeof(RoleLogic)) ||
					IsBadReadPtr(player, sizeof(Role)) ||
					IsBadReadPtr(player->MyRoleControl, sizeof(RoleControl)) ||
					Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) continue;
				
				auto pHead = player->MyRoleControl->animatorControl->animator->GetBoneTransform(II::Animator::HumanBodyBones::Head);

				auto xy = camera->WorldToScreenPoint(pHead->GetPosition() + II::Vector3{0, 0.15, 0}, II::Camera::Eye::Mono);
				if (xy.z < 0) continue;
				xy.y = static_cast<float>(windowHeight) - xy.y;

				int oLen = (int)sqrt(pow(xy.x - (windowWidth / 2), 2) + pow(xy.y - (windowHeight / 2), 2));
				if ((xy.x > (windowWidth / 2 - rangeX)) && (xy.x < (windowWidth / 2 + rangeX)) && (xy.y > (windowHeight / 2 - rangeY)) && (xy.y < (windowHeight / 2 + rangeY))) {
					if (oLen < len) {
						len = oLen;
						screenPoint = DrawHelp::BoxScreen::ToVec2(xy);
						hasObj = true;
						aimPlayer = player;
					}
				}
			}
		}
		catch (...) {

		}

		if (hasObj) {
			if (GetKeyState(aimKey) <= -127) {
				unsigned long MouseX = (screenPoint.x - (windowWidth / 2)) / xSpeed;
				unsigned long MouseY = (screenPoint.y - (windowHeight / 2)) / ySpeed;
				mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
			}
		}
	} else {
		hasObj = false;
	}
}
void PlayerAim::Save(nlohmann::json& json) {
	json["PlayerAim"]["enable"] = enable;
	json["PlayerAim"]["xSpeed"] = xSpeed;
	json["PlayerAim"]["ySpeed"] = ySpeed;
	json["PlayerAim"]["aimKey"] = aimKey;
	json["PlayerAim"]["keyName"] = keyName;
	json["PlayerAim"]["rangeX"] = rangeX;
	json["PlayerAim"]["rangeY"] = rangeY;
}
void PlayerAim::Load(nlohmann::json& json) {
	if (json.contains("PlayerAim")) {
		if (json["PlayerAim"].contains("enable")) {
			enable = json["PlayerAim"]["enable"];
		}
		if (json["PlayerAim"].contains("xSpeed")) {
			xSpeed = json["PlayerAim"]["xSpeed"];
		}
		if (json["PlayerAim"].contains("ySpeed")) {
			ySpeed = json["PlayerAim"]["ySpeed"];
		}
		if (json["PlayerAim"].contains("aimKey")) {
			aimKey = json["PlayerAim"]["aimKey"];
		}
		if (json["PlayerAim"].contains("keyName")) {
			keyName = json["PlayerAim"]["keyName"];
		}
		if (json["PlayerAim"].contains("rangeX")) {
			rangeX = json["PlayerAim"]["rangeX"];
		}
		if (json["PlayerAim"].contains("rangeY")) {
			rangeY = json["PlayerAim"]["rangeY"];
		}
	}
}
auto PlayerAim::GetInstance() -> PlayerAim* {
	static PlayerAim PlayerAim;
	return &PlayerAim;
}
