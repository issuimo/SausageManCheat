#include "PlayerAim.h"
#include <WinUser.h>
#include "../../../GameDefine/Role/Role.h"
auto PlayerAim::GetInfo() const -> const GuiInfo& { return *new GuiInfo{reinterpret_cast<const char*>(u8"自瞄"), reinterpret_cast<const char*>(u8"玩家"), false, true, true}; };

auto PlayerAim::Draw() -> void {
	const auto bg = ImGui::GetBackgroundDrawList();
	if (hasObj) {
		bg->AddCircleFilled(screenPoint, 5, ImColor{255, 0, 0}, 15);
		if (teleport) DrawTextWithOutline(bg, screenPoint, reinterpret_cast<const char*>(u8"按T键瞬移"), ImColor{255, 0, 0, 200}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
	}
	if (enable) bg->AddRect({(windowWidth / 2 - rangeX), (windowHeight / 2 - rangeY)}, {(windowWidth / 2 + rangeX), (windowHeight / 2 + rangeY)}, ImColor{0, 255, 0}, 0, 0, 2);
}

auto PlayerAim::Render() -> void {
	ImGui::Checkbox(reinterpret_cast<const char*>(u8"启用"), &enable);
	if (enable && ImGui::BeginTable("PlayerEsp", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"值"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("速度Xch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"水平速度");
		if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"分之一", &xSpeed, 3, 25);
		ImGui::PopID();

		ImGui::PushID("速度Ych");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"垂直速度");
		if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"分之一", &ySpeed, 3, 25);
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
				if (ImGui::Selectable("Shift")) {
					aimKey  = 16;
					keyName = "Shift";
				}
				if (ImGui::Selectable("RButton")) {
					aimKey  = 2;
					keyName = "RButton";
				}
				if (ImGui::Selectable("LButton")) {
					aimKey  = 1;
					keyName = "LButton";
				}
				if (ImGui::Selectable("Ctrl")) {
					aimKey  = 17;
					keyName = "Ctrl";
				}
				ImGui::EndCombo();
			}
		}
		ImGui::PopID();

		ImGui::PushID("传送ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"T键传送");
		if (ImGui::TableSetColumnIndex(1)) ImGui::Checkbox((const char*)u8"启用", &teleport);
		ImGui::PopID();

		if (teleport) {
			ImGui::PushID("安全距离ch");
			ImGui::TableNextRow();
			if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"安全距离");
			if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"米", &dis, 0, 50);
			ImGui::PopID();
		}

		ImGui::PushID("锁定ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"自瞄锁定");
		if (ImGui::TableSetColumnIndex(1)) ImGui::Checkbox((const char*)u8"启用", &aimLock);
		ImGui::PopID();

		ImGui::PushID("手动瞄准ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"模拟手瞄");
		if (ImGui::TableSetColumnIndex(1)) ImGui::Checkbox((const char*)u8"启用", &readyAim);
		ImGui::PopID();

		if (readyAim) {
			ImGui::PushID("模拟值ch");
			ImGui::TableNextRow();
			if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"模拟值");
			if (ImGui::TableSetColumnIndex(1)) ImGui::SliderFloat((const char*)u8"%抖动", &readyValue, 1, 50);
			ImGui::PopID();
		}

		ImGui::PushID("倒地ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text((const char*)u8"自瞄倒地");
		if (ImGui::TableSetColumnIndex(1)) ImGui::Checkbox(reinterpret_cast<const char*>(u8"启用"), &aimWeak);
		ImGui::PopID();

		ImGui::EndTable();
	}
}

auto PlayerAim::Update() -> void {
	if (enable) {
		std::lock_guard lock(Role::mutex);
		hasObj    = false;
		float len = 99999;
		[&]() {
			__try {
				[&]() {
					if (aimLock && aimPlayer && GetKeyState(aimKey) <= -127 && std::find(Role::vector.begin(), Role::vector.end(), aimPlayer) == Role::vector.end()) {
						aimPlayer = nullptr;
						hasObj    = false;
					}

					for (const auto& player : Role::vector) {
						if (player == Role::localRole || IsBadReadPtr(Role::localRole, sizeof(RoleLogic)) || IsBadReadPtr(player, sizeof(Role)) || IsBadReadPtr(player->MyRoleControl, sizeof(RoleControl)) || Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) continue;

						II::Transform* pHead;

						II::Vector3 xy;

						bool weak{};

						if (player->roleLogic->hp < 0 && player->roleLogic->WeakValue < 100) weak = true;

						if (weak && !aimWeak) continue;

						[&]() {
							__try {
								pHead = player->MyRoleControl->animatorControl->animator->GetBoneTransform(II::Animator::HumanBodyBones::Head);
								if (!weak) xy = WorldToScreenPoint(pHead->GetPosition() + II::Vector3{0, 0.15, 0});
								else xy       = WorldToScreenPoint(pHead->GetPosition() - II::Vector3{0, 1, 0});
							} __except (EXCEPTION_EXECUTE_HANDLER) { []() { ERROR("RoleAim-GetData Error (except)"); }(); }
						}();

						if (xy.z < 0 && player == aimPlayer && (xy.x > windowWidth || xy.x < 0) && (xy.y > windowHeight || xy.y < 0)) {
							aimPlayer = nullptr;
							hasObj    = false;
						}
						if (xy.z < 0) continue;
						if (aimLock && aimPlayer && GetKeyState(aimKey) <= -127 && player != aimPlayer) continue;

						const int oLen = static_cast<int>(sqrt(pow(xy.x - (windowWidth / 2), 2) + pow(xy.y - (windowHeight / 2), 2)));
						if ((xy.x > (windowWidth / 2 - rangeX)) && (xy.x < (windowWidth / 2 + rangeX)) && (xy.y > (windowHeight / 2 - rangeY)) && (xy.y < (windowHeight / 2 + rangeY))) {
							if (oLen < len) {
								len         = oLen;
								screenPoint = DrawHelp::BoxScreen::ToVec2(xy);
								hasObj      = true;
								aimPlayer   = player;
							}
						}
					}

					if (hasObj) {
						if (GetKeyState(aimKey) <= -127) {
							if (!readyAim) {
								const unsigned long MouseX = (screenPoint.x - (windowWidth / 2)) / xSpeed;
								const unsigned long MouseY = (screenPoint.y - (windowHeight / 2)) / ySpeed;
								mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
							} else {
								srand(time(nullptr) * 1000 + screenPoint.x + screenPoint.y);
								const float         tmpSpeed = xSpeed + (rand() % static_cast<int>(readyValue));
								const unsigned long MouseX   = (screenPoint.x - (windowWidth / 2)) / tmpSpeed;
								const unsigned long MouseY   = (screenPoint.y - (windowHeight / 2)) / tmpSpeed;
								mouse_event(MOUSEEVENTF_MOVE, MouseX, MouseY, 0, 0);
							}
						}
						if (teleport && GetKeyState(0x54) <= -127 && aimPlayer) {
							[&]() {
								__try {
									[&]() {
										const float       fovX      = -aimPlayer->MyRoleControl->y_camRot;
										const II::Vector3 point     = aimPlayer->roleLogic->NowPoint;
										II::Vector3       topPoint1 = {point.x, point.y, point.z - dis};
										RotatePoint(topPoint1.x, topPoint1.z, point.x, point.z, fovX, topPoint1.x, topPoint1.z);
										Role::localRole->GetTransform()->SetPosition(topPoint1);
									}();
								} __except (EXCEPTION_EXECUTE_HANDLER) { []() { ERROR("PlayerAim Error (except)"); }(); }
							}();
						}
					}
				}();
			} __except (EXCEPTION_EXECUTE_HANDLER) { []() { ERROR("PlayerAim Error (except)"); }(); }
		}();
	} else hasObj = false;
}

auto PlayerAim::Save(nlohmann::json& json) -> void {
	json["PlayerAim"]["enable"]     = enable;
	json["PlayerAim"]["xSpeed"]     = xSpeed;
	json["PlayerAim"]["ySpeed"]     = ySpeed;
	json["PlayerAim"]["aimKey"]     = aimKey;
	json["PlayerAim"]["keyName"]    = keyName;
	json["PlayerAim"]["rangeX"]     = rangeX;
	json["PlayerAim"]["rangeY"]     = rangeY;
	json["PlayerAim"]["teleport"]   = teleport;
	json["PlayerAim"]["readyAim"]   = readyAim;
	json["PlayerAim"]["readyValue"] = readyValue;
	json["PlayerAim"]["aimLock"]    = aimLock;
	json["PlayerAim"]["aimWeak"] = aimWeak;
	json["PlayerAim"]["dis"]    = dis;
}

auto PlayerAim::Load(nlohmann::json& json) -> void {
	if (json.contains("PlayerAim")) {
		if (json["PlayerAim"].contains("enable")) enable = json["PlayerAim"]["enable"];
		if (json["PlayerAim"].contains("xSpeed")) xSpeed = json["PlayerAim"]["xSpeed"];
		if (json["PlayerAim"].contains("ySpeed")) ySpeed = json["PlayerAim"]["ySpeed"];
		if (json["PlayerAim"].contains("aimKey")) aimKey = json["PlayerAim"]["aimKey"];
		if (json["PlayerAim"].contains("keyName")) keyName = json["PlayerAim"]["keyName"];
		if (json["PlayerAim"].contains("rangeX")) rangeX = json["PlayerAim"]["rangeX"];
		if (json["PlayerAim"].contains("rangeY")) rangeY = json["PlayerAim"]["rangeY"];
		if (json["PlayerAim"].contains("teleport")) teleport = json["PlayerAim"]["teleport"];
		if (json["PlayerAim"].contains("readyAim")) readyAim = json["PlayerAim"]["readyAim"];
		if (json["PlayerAim"].contains("readyValue")) readyValue = json["PlayerAim"]["readyValue"];
		if (json["PlayerAim"].contains("aimLock")) aimLock = json["PlayerAim"]["aimLock"];
		if (json["PlayerAim"].contains("aimWeak")) aimWeak = json["PlayerAim"]["aimWeak"];
		if (json["PlayerAim"].contains("dis")) dis = json["PlayerAim"]["dis"];
	}
}

auto PlayerAim::GetInstance() -> PlayerAim* {
	static PlayerAim PlayerAim;
	return &PlayerAim;
}
