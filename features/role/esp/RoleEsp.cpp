﻿#include "RoleEsp.h"

#include "../../../CLASS/Role.h"
#include "../../../CLASS/PickItemNet.h"
#include "../../../CLASS/PickItem.h"
#include "../../../CLASS/PickItemDataConfig.h"

#include "Base/BaseMath.h"


static auto GetTeamColor(const unsigned long& teamId, const int& a) -> ImColor;
static auto DrawQuadLines(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImU32& col) -> void;
void DrawBox(II::Camera*& cam, const II::Vector3& point, const II::Vector3& sPoint, const float& height, const int& team, const float& radius, const float& radiusZ, float& fovX);
void DrawLine(II::Camera*& cam, const II::Vector3& point, const float& height, const int& team);
void DrawBone(Role* player, II::Camera*& cam, const int& team);
void DrawPlayerInfo(Role* player, II::Camera*& cam, const int& team, bool weak, const II::Vector3& point, const float& height, const II::Vector3& mPoint);
void DrawView(const II::Vector3& point, II::Camera*& cam, const int& team, const float& height, const float& radius, float& fovX);

cheat::feature::RoleESp* cheat::feature::RoleESp::GetInstance() {
	static RoleESp role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::RoleESp::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "透视", "玩家", true };
	return info;
}
void cheat::feature::RoleESp::DrawMain() {
	ConfigWidget(f_Enable);
	if (f_Enable) {
		ConfigWidget(f_Box);
		ImGui::SameLine();
		ConfigWidget(f_Info);
		ImGui::SameLine();
		ConfigWidget(f_Equip);
		ImGui::SameLine();
		ConfigWidget(f_Line);
		ConfigWidget(f_Bone);
		ImGui::SameLine();
		ConfigWidget(f_Weak);
		ImGui::SameLine();
		ConfigWidget(f_View);
		ImGui::SameLine();
		ConfigWidget(f_Solid, "立体方框");
	}
}
bool cheat::feature::RoleESp::NeedStatusDraw() const { return f_Enable; }
void cheat::feature::RoleESp::DrawStatus() {
	ImGui::Text("玩家透视");
}
bool cheat::feature::RoleESp::NeedInfoDraw() const { return f_Enable; }
void cheat::feature::RoleESp::DrawInfo() { ImGui::Text("玩家数量[%d]", Role::vector.GetValue().size()); }
void cheat::feature::RoleESp::DrawExternal() {
	if (f_Enable) {
		_set_se_translator(SEH_exception::TranslateSEHtoCE);
		if (Role::localRole) {

			static II::Vector3 point;
			static II::Vector3 scale;
			static II::Vector3 pHead;
			static float       height;
			static int         team;
			static bool        weak;
			static float       radius;
			static float       radiusZ;
			static float       fovX;
			static II::Vector3 mPoint;
			static II::Vector3 sPoint;
			static II::Camera* cam;
			static RoleLogic* roleLogic;
			static RoleControl* roleControl;

			try {
				if (const auto cams = II::Camera::GetAllCamera(); !cams.empty()) {
					cam = cams[0];
				}
			} catch (...) {
				LOG_ERROR("error");
				return;
			}

			for (const auto& player : Role::vector.GetValue()) {
				try {
					if (IsBadReadPtr(player, sizeof(void*)) 
						|| IsBadReadPtr(Role::roleLogic.Get(player), sizeof(void*))
						|| player == Role::localRole 
						|| RoleLogic::TeamNum.Get(Role::roleLogic.Get(player)) == RoleLogic::TeamNum.Get(Role::roleLogic.Get(Role::localRole)))
						continue;
				} catch (...) {
					LOG_ERROR("error");
					continue;
				}

				try {
					roleLogic = Role::roleLogic.Get(player);
					roleControl = Role::MyRoleControl.Get(player);

					if (RoleLogic::hp.Get(roleLogic) < 0 && RoleLogic::WeakValue.Get(roleLogic) < 100) {
						height = 0.5;
						radius = 0.3;
						radiusZ = 0.3;
						weak = true;
					}

					if (weak && !f_Weak) return;

					point = RoleLogic::NowPoint.Get(roleLogic);
					team = RoleLogic::TeamNum.Get(roleLogic);

					scale = player->GetTransform()->GetLocalScale();
					radius = 0.5 * scale.x;
					radiusZ = 0.5 * scale.z;

					pHead = AnimatorControl::animator.Get(RoleControl::animatorControl.Get(Role::MyRoleControl.Get(player)))->GetBoneTransform(II::Animator::HumanBodyBones::Head)->GetPosition();
					height = pHead.y - point.y + 0.6 * scale.y;

					fovX = -RoleControl::y_camRot.Get(roleControl);

					mPoint = RoleLogic::NowPoint.Get(Role::roleLogic.Get(Role::localRole));

					sPoint = cam->WorldToScreenPoint(point);
					sPoint.y = renderer::windowHeight - sPoint.y;
				} catch (...) {
					LOG_ERROR("error");
					continue;
				}

				try {
					if (f_Box) {
						DrawBox(cam, point, sPoint, height, team, radius, radiusZ, fovX);
					}

					if (f_Line) {
						DrawLine(cam, point, height, team);
					}

					if (f_Bone && !weak) {
						DrawBone(player, cam, team);
					}

					if (f_Info) {
						DrawPlayerInfo(player, cam, team, weak, point, height, mPoint);
					}

					if (f_View) {
						DrawView(point, cam, team, height, radius, fovX);
					}
				} catch (...) { LOG_ERROR("error"); }
			}
		}
	}
}
cheat::feature::RoleESp::RoleESp() :
NF(f_Enable, "透视", "Role::Esp", false),
NF(f_Box, "方框", "Role::Box", false),
NF(f_Info, "信息", "Role::Info", false),
NF(f_Equip, "装备", "Role::Equip", false),
NF(f_Line, "射线", "Role::Line", false),
NF(f_Bone, "骨骼", "Role::Bone", false),
NF(f_Weak, "倒地", "Role::Weak", false),
NF(f_View, "视角", "Role::View", false),
NF(f_Solid, "立体", "Role::Solid", false) {
	
}

struct BoxScreen {
	ImVec2 lowerTopLeft;
	ImVec2 lowerTopRight;
	ImVec2 lowerBottomLeft;
	ImVec2 lowerBottomRight;

	ImVec2 upperTopLeft;
	ImVec2 upperTopRight;
	ImVec2 upperBottomLeft;
	ImVec2 upperBottomRight;

	static auto ToVec2(II::Vector3 vector3) -> ImVec2 { return { vector3.x, vector3.y }; }
};

ImVec2 ConvertWorldToScreen(const II::Vector3& worldPoint, II::Camera* cam) {
	auto screenPoint = cam->WorldToScreenPoint(worldPoint);
	screenPoint.y = renderer::windowHeight - screenPoint.y;
	return BoxScreen::ToVec2(screenPoint);
}

void DrawView(const II::Vector3& point, II::Camera*& cam, const int& team, const float& height, const float& radius, float& fovX) {
	if (point.z <= 0) {
		return;
	}

	const auto bg = ImGui::GetBackgroundDrawList();
	const ImVec2 baseScreenPoint = ConvertWorldToScreen(point, cam);

	// Calculate top points
	II::Vector3 topPoint = { point.x, point.y + height - 0.3f, point.z + radius };
	ImVec2 topScreenPoint;
	BaseMath::RotatePoint({ topPoint.x, topPoint.z }, { point.x, point.z }, fovX, topScreenPoint);
	topPoint.x = topScreenPoint.x;
	topPoint.z = topScreenPoint.y;

	try {
		// Convert world space to screen space and draw
		const ImVec2 pos1 = ConvertWorldToScreen(topPoint, cam);
		const ImVec2 pos2 = baseScreenPoint;

		bg->AddLine(pos1, pos2, GetTeamColor(team, 200));
		bg->AddCircleFilled(pos1, 3, GetTeamColor(team, 200));
	}
	catch (const std::exception& e) {
		LOG_ERROR("Exception caught: %s", e.what());
	}
}


void DrawPlayerInfo(Role* player, II::Camera*& cam, const int& team, bool weak, const II::Vector3& point, const float& height, const II::Vector3& mPoint) {
	const auto bg = ImGui::GetBackgroundDrawList();
	II::Vector3 p1;

	try {
		p1 = cam->WorldToScreenPoint({ point.x, point.y + height, point.z });
	}
	catch (...) {
		LOG_ERROR("error");
		return;
	}

	p1.y = renderer::windowHeight - p1.y;
	if (p1.z < 0) return;

	auto xy = BoxScreen::ToVec2(p1);

	bg->AddRectFilled({ xy.x - 100.0f, xy.y }, { xy.x + 100.0f, xy.y - 30.0f }, GetTeamColor(team, 150), 0);
	bg->AddRectFilled({ xy.x - 100.0f, xy.y }, { xy.x - 70.0f, xy.y - 30.0f }, GetTeamColor(team, 255), 0);
	bg->AddLine({ xy.x - 100.0f, xy.y }, { xy.x + 100.0f, xy.y }, ImColor{ 0, 0, 0 }, 3);

	try {
		float hp = RoleLogic::hp.Get(Role::roleLogic.Get(player));
		float maxHp = RoleLogic::maxHp.Get(Role::roleLogic.Get(player));
		float hpWidth = 200 * (hp / maxHp);

		bg->AddLine({ xy.x - 100.0f, xy.y }, { xy.x - 100.0f + hpWidth, xy.y }, ImColor{ 255, 255, 255 }, 3);

		std::string playerName = RoleLogic::NickName.Get(Role::roleLogic.Get(player))->ToString();
		std::string infoText = std::format("{:02} | {}", team, playerName);
		DrawTextWithOutline(bg, { xy.x - 95.0f, xy.y - 25.0f }, infoText.c_str(), ImColor{ 255, 255, 255 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
	}
	catch (...) {
		LOG_ERROR("error");
		return;
	}

	if (weak) {
		DrawTextWithOutline(bg, { xy.x - 95.0f, xy.y + 10.0f }, "倒地", ImColor{ 255, 255, 255 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
	}

	try {
		std::string distanceKillsText = std::format("{}m | {} kills",
			static_cast<int>(sqrt(pow(point.x - mPoint.x, 2) + pow(point.y - mPoint.y, 2) + pow(point.z - mPoint.z, 2))),
			RoleLogic::KillRoleNum.Get(Role::roleLogic.Get(player)));

		if (cheat::feature::RoleESp::GetInstance()->f_Equip) {
			std::string playerWeaponName = RoleLogic::UserWeapon.Get(Role::roleLogic.Get(player)) ?
				PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(RoleLogic::UserWeapon.Get(Role::roleLogic.Get(player))))->ToString() :
				"空手";
			int bullet = RoleLogic::UserWeapon.Get(Role::roleLogic.Get(player)) ?
				PickItemNet::BulletNum.Get(RoleLogic::UserWeapon.Get(Role::roleLogic.Get(player))) : 0;

			distanceKillsText += std::format(" | {}:{}", bullet, playerWeaponName);
		}

		DrawTextWithOutline(bg, { xy.x - 100.0f, xy.y - 45.0f }, distanceKillsText.c_str(), ImColor{ 255, 255, 255 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
	}
	catch (...) {
		LOG_ERROR("error");
		return;
	}

	if (cheat::feature::RoleESp::GetInstance()->f_Equip) {
		try {
			const std::string headPart = RoleLogic::HeadEquipPart.Get(Role::roleLogic.Get(player)) ?
				PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(RoleLogic::HeadEquipPart.Get(Role::roleLogic.Get(player))))->ToString() : "无";
			const std::string bodyPart = RoleLogic::BodyEquipPart.Get(Role::roleLogic.Get(player)) ?
				PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(RoleLogic::BodyEquipPart.Get(Role::roleLogic.Get(player))))->ToString() : "无";
			const std::string packPart = RoleLogic::PackEquip.Get(Role::roleLogic.Get(player)) ?
				PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(RoleLogic::PackEquip.Get(Role::roleLogic.Get(player))))->ToString() : "无";

			DrawTextWithOutline(bg, { xy.x + 101.0f, xy.y }, headPart.c_str(), ImColor{ 255, 255, 255 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
			DrawTextWithOutline(bg, { xy.x + 101.0f, xy.y - 15.0f }, bodyPart.c_str(), ImColor{ 255, 255, 255 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
			DrawTextWithOutline(bg, { xy.x + 101.0f, xy.y - 30.0f }, packPart.c_str(), ImColor{ 255, 255, 255 }, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
		}
		catch (...) {
			LOG_ERROR("error");
		}
	}
}


ImVec2 ConvertToImVec2(const II::Vector3& vec3) {
	return ImVec2(vec3.x, renderer::windowHeight - vec3.y);
}

bool IsPointOnScreen(const II::Vector3& point) {
	return point.z > 0;
}

void DrawBone(Role* player, II::Camera*& cam, const int& team) {
	try {
		const auto animator = AnimatorControl::animator.Get(RoleControl::animatorControl.Get(Role::MyRoleControl.Get(player)));
		if (animator) {
			// 定义骨骼点数组
			std::array bones = {
				animator->GetBoneTransform(II::Animator::HumanBodyBones::Head)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::Chest)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::Hips)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperArm)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperArm)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerArm)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerArm)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperLeg)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperLeg)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerLeg)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerLeg)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::RightHand)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftHand)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::RightFoot)->GetPosition(),
				animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftFoot)->GetPosition()
			};

			// 转换屏幕坐标并验证是否所有点都在屏幕上
			std::array<II::Vector3, 15> screenPoints;
			bool allPointsOnScreen = true;
			std::ranges::transform(bones, screenPoints.begin(), [&](const II::Vector3& bonePos) {
				auto point = cam->WorldToScreenPoint(bonePos);
				if (!IsPointOnScreen(point)) {
					allPointsOnScreen = false;
				}
				return point;
			});

			if (allPointsOnScreen) {
				const auto bg = ImGui::GetBackgroundDrawList();

				// 绘制骨架连接线
				const auto& [pHead, pChest, pHips, pLeftArmU, pRightArmU, pLeftArmL, pRightArmL,
					pLeftLegU, pRightLegU, pLeftLegL, pRightLegL, pRightHand, pLeftHand,
					pRightFoot, pLeftFoot] = screenPoints;

				bg->AddLine(ConvertToImVec2(pHead), ConvertToImVec2(pChest), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pChest), ConvertToImVec2(pLeftArmU), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pChest), ConvertToImVec2(pRightArmU), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pLeftArmU), ConvertToImVec2(pLeftArmL), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pRightArmU), ConvertToImVec2(pRightArmL), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pLeftArmL), ConvertToImVec2(pLeftHand), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pRightArmL), ConvertToImVec2(pRightHand), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pChest), ConvertToImVec2(pHips), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pHips), ConvertToImVec2(pLeftLegU), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pHips), ConvertToImVec2(pRightLegU), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pLeftLegU), ConvertToImVec2(pLeftLegL), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pRightLegU), ConvertToImVec2(pRightLegL), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pLeftLegL), ConvertToImVec2(pLeftFoot), GetTeamColor(team, 255), 1);
				bg->AddLine(ConvertToImVec2(pRightLegL), ConvertToImVec2(pRightFoot), GetTeamColor(team, 255), 1);
			}
		}
	}
	catch (...) {
		LOG_ERROR("error");
	}
}


void DrawLine(II::Camera*& cam, const II::Vector3& point, const float& height, const int& team) {
	try {
		auto p1 = cam->WorldToScreenPoint({ point.x, point.y + height, point.z });
		if (p1.z < 0) return;
		p1.y = renderer::windowHeight - p1.y;
		const ImVec2 xy = { p1.x, p1.y };

		const auto bg = ImGui::GetBackgroundDrawList();
		bg->AddLine({ renderer::windowWidth * 0.5f, 0 }, xy, GetTeamColor(team, 200), 1);
	}
	catch (...) {
		LOG_ERROR("error");
	}
}


void DrawBox(II::Camera*& cam, const II::Vector3& point, const II::Vector3& sPoint, const float& height, const int& team, const float& radius, const float& radiusZ, float& fovX) {
	const auto roleEsp = cheat::feature::RoleESp::GetInstance();
	if (!roleEsp->f_Solid) {
		try {
			auto xy = cam->WorldToScreenPoint({ point.x, point.y + height, point.z });
			xy.y = renderer::windowHeight - xy.y;
			if (xy.z < 0) return;
			const float w = (sPoint.y - xy.y) / 3;

			ImVec2 p1 = { xy.x, xy.y }, p2 = { xy.x, xy.y }, p3 = { sPoint.x, sPoint.y }, p4 = { sPoint.x, sPoint.y };
			p1.x -= w;
			p2.x += w;
			p3.x += w;
			p4.x -= w;

			DrawQuadLines(p1, p2, p3, p4, GetTeamColor(team, 200));
		}
		catch (...) {
			LOG_ERROR("error");
		}
	}
	else {
		auto draw3DBox = [&](const II::Vector3& point, const ImColor& color) {
			BoxScreen box;
			auto calculatePoint = [&](const II::Vector3& offset) {
				II::Vector3 rotatedPoint = { point.x + offset.x, point.y + offset.y, point.z + offset.z };
				ImVec2 out;
				BaseMath::RotatePoint({ rotatedPoint.x, rotatedPoint.z }, { point.x, point.z }, fovX, out);
				rotatedPoint.x = out.x;
				rotatedPoint.z = out.y;
				auto screenPoint = cam->WorldToScreenPoint(rotatedPoint);
				screenPoint.y = renderer::windowHeight - screenPoint.y;
				return screenPoint;
				};

			auto checkAndAssign = [&](ImVec2& boxPoint, const II::Vector3& offset) {
				auto screenPoint = calculatePoint(offset);
				if (screenPoint.z < 0) return false;
				boxPoint = { screenPoint.x, screenPoint.y };
				return true;
				};

			if (!checkAndAssign(box.lowerTopLeft, { -radius, 0, -radiusZ }) ||
				!checkAndAssign(box.lowerTopRight, { -radius, 0, radiusZ }) ||
				!checkAndAssign(box.lowerBottomLeft, { radius, 0, -radiusZ }) ||
				!checkAndAssign(box.lowerBottomRight, { radius, 0, radiusZ }) ||
				!checkAndAssign(box.upperTopLeft, { -radius, height, -radiusZ }) ||
				!checkAndAssign(box.upperTopRight, { -radius, height, radiusZ }) ||
				!checkAndAssign(box.upperBottomLeft, { radius, height, -radiusZ }) ||
				!checkAndAssign(box.upperBottomRight, { radius, height, radiusZ })) {
				return;
			}

			DrawQuadLines(box.lowerBottomLeft, box.lowerTopLeft, box.lowerTopRight, box.lowerBottomRight, color);
			DrawQuadLines(box.upperBottomLeft, box.upperTopLeft, box.upperTopRight, box.upperBottomRight, color);

			auto bg = ImGui::GetBackgroundDrawList();
			bg->AddLine(box.lowerBottomLeft, box.upperBottomLeft, color);
			bg->AddLine(box.lowerTopLeft, box.upperTopLeft, color);
			bg->AddLine(box.lowerTopRight, box.upperTopRight, color);
			bg->AddLine(box.lowerBottomRight, box.upperBottomRight, color);
			};

		try {
			draw3DBox(point, GetTeamColor(team, 200));
		}
		catch (...) {
			LOG_ERROR("error");
		}
	}
}


static auto GetTeamColor(const unsigned long& teamId, const int& a) -> ImColor {
	switch (teamId) {
	case 0: return { 0, 0, 0, a };
	case 1: return { 133, 151, 21, a };
	case 2: return { 173, 29, 210, a };
	case 3: return { 148, 221, 196, a };
	case 4: return { 118, 25, 57, a };
	case 5: return { 49, 241, 173, a };
	case 6: return { 181, 88, 240, a };
	case 7: return { 147, 151, 50, a };
	case 8: return { 25, 43, 209, a };
	case 9: return { 192, 253, 22, a };
	case 10: return { 142, 78, 72, a };
	case 11: return { 155, 11, 245, a };
	case 12: return { 59, 73, 168, a };
	case 13: return { 99, 93, 222, a };
	case 14: return { 63, 223, 109, a };
	case 15: return { 104, 180, 135, a };
	case 16: return { 154, 170, 205, a };
	case 17: return { 220, 247, 193, a };
	case 18: return { 68, 129, 41, a };
	case 19: return { 8, 27, 64, a };
	case 20: return { 98, 56, 48, a };
	case 21: return { 78, 148, 212, a };
	case 22: return { 17, 208, 222, a };
	case 23: return { 196, 17, 157, a };
	case 24: return { 75, 63, 156, a };
	case 25: return { 70, 187, 239, a };
	case 26: return { 199, 84, 33, a };
	case 27: return { 80, 43, 208, a };
	case 28: return { 239, 90, 244, a };
	case 29: return { 9, 207, 95, a };
	case 30: return { 53, 145, 148, a };
	case 31: return { 54, 127, 137, a };
	case 32: return { 112, 153, 177, a };
	case 33: return { 30, 103, 204, a };
	case 34: return { 17, 84, 3, a };
	case 35: return { 127, 156, 3, a };
	case 36: return { 74, 246, 155, a };
	case 37: return { 30, 237, 103, a };
	case 38: return { 119, 59, 194, a };
	case 39: return { 164, 206, 80, a };
	case 40: return { 116, 249, 198, a };
	case 41: return { 187, 122, 88, a };
	case 42: return { 162, 134, 69, a };
	case 43: return { 179, 147, 232, a };
	case 44: return { 190, 170, 208, a };
	case 45: return { 15, 239, 102, a };
	case 46: return { 232, 28, 0, a };
	case 47: return { 197, 87, 112, a };
	case 48: return { 102, 183, 88, a };
	case 49: return { 38, 87, 232, a };
	case 50: return { 251, 224, 129, a };
	case 51: return { 159, 119, 199, a };
	case 52: return { 251, 230, 193, a };
	case 53: return { 205, 124, 235, a };
	case 54: return { 94, 54, 203, a };
	case 55: return { 166, 117, 86, a };
	case 56: return { 118, 188, 40, a };
	case 57: return { 156, 199, 163, a };
	case 58: return { 36, 207, 244, a };
	case 59: return { 7, 119, 152, a };
	case 60: return { 150, 109, 163, a };
	case 61: return { 65, 92, 23, a };
	case 62: return { 240, 188, 1, a };
	case 63: return { 39, 6, 231, a };
	case 64: return { 123, 7, 186, a };
	case 65: return { 14, 118, 7, a };
	case 66: return { 41, 177, 0, a };
	case 67: return { 72, 3, 136, a };
	case 68: return { 134, 218, 42, a };
	case 69: return { 197, 255, 33, a };
	case 70: return { 124, 153, 103, a };
	case 71: return { 125, 236, 249, a };
	case 72: return { 111, 41, 216, a };
	case 73: return { 162, 115, 100, a };
	case 74: return { 151, 91, 172, a };
	case 75: return { 207, 81, 122, a };
	case 76: return { 167, 23, 19, a };
	case 77: return { 169, 245, 211, a };
	case 78: return { 34, 234, 37, a };
	case 79: return { 176, 144, 217, a };
	case 80: return { 239, 203, 14, a };
	case 81: return { 49, 178, 94, a };
	case 82: return { 1, 146, 33, a };
	case 83: return { 159, 216, 130, a };
	case 84: return { 56, 229, 156, a };
	case 85: return { 177, 118, 13, a };
	case 86: return { 200, 129, 70, a };
	case 87: return { 28, 92, 197, a };
	case 88: return { 190, 37, 45, a };
	case 89: return { 201, 139, 110, a };
	case 90: return { 127, 148, 44, a };
	case 91: return { 206, 213, 105, a };
	case 92: return { 95, 89, 8, a };
	case 93: return { 148, 217, 64, a };
	case 94: return { 151, 99, 58, a };
	case 95: return { 242, 62, 114, a };
	case 96: return { 64, 135, 202, a };
	case 97: return { 43, 59, 95, a };
	case 98: return { 43, 142, 193, a };
	case 99: return { 6, 203, 46, a };
	case 100: return { 162, 91, 30, a };
	default: return 0;
	}
}

static auto DrawQuadLines(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImU32& col) -> void {
	const auto draw = ImGui::GetBackgroundDrawList();
	draw->AddLine(p1, p2, col);
	draw->AddLine(p2, p3, col);
	draw->AddLine(p3, p4, col);
	draw->AddLine(p4, p1, col);
}