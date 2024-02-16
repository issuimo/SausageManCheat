﻿#include "PlayerEsp.h"
#include "../../../GameDefine/Role/Role.h"

auto PlayerEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>("玩家"), reinterpret_cast<const char*>("透视"), true, false, true }; }
auto PlayerEsp::Draw() -> void {
	if (enable) {
		const auto bg = ImGui::GetBackgroundDrawList();
		std::lock_guard lock(Role::mutex);
		for (const auto& player : Role::vector) {
			try {
				if (Role::localRole && player->roleLogic->isLocalRole && player->roleLogic->teamNum == Role::localRole->roleLogic->teamNum) continue;
				const auto camera = II::Camera::GetAllCamera()[0];
				auto       point  = player->roleLogic->nowPoint;
				const int  team   = player->roleLogic->teamNum;
				const float height = player->roleControl->height;
				const float radius = player->roleControl->radius / 2;
				if (auto sPoint = camera->WorldToScreenPoint(point, II::Camera::Eye::Mono);
					sPoint.z > -1 && (sPoint.y < static_cast<float>(windowHeight) && sPoint.y > 0) && (sPoint.x < static_cast<float>(windowWidth) && sPoint.x > 0)) {
					sPoint.y = static_cast<float>(windowHeight) - sPoint.y;
					if (setting.box) {
						switch (setting.boxStyle) {
						case BoxStyle::Quad: {
							auto  xy = camera->WorldToScreenPoint({ point.x, point.y + height, point.z }, II::Camera::Eye::Mono);
							if (xy.z < 0 && (xy.y > static_cast<float>(windowHeight) && xy.y < 0) && (xy.x < static_cast<float>(windowWidth) && xy.x > 0)) continue;
							xy.y = static_cast<float>(windowHeight) - xy.y;
							const float w  = (sPoint.y - xy.y) / 3;

							ImVec2 p1 = DrawHelp::BoxScreen::ToVec2(xy), p2 = DrawHelp::BoxScreen::ToVec2(xy), p3 = DrawHelp::BoxScreen::ToVec2(sPoint), p4 =DrawHelp::BoxScreen::ToVec2(sPoint);
							p1.x -= w;
							p2.x += w;
							p3.x += w;
							p4.x -= w;

							DrawHelp::DrawQuadLines(p1, p2, p3, p4, DrawHelp::GetTeamColor(team, 200));
							break;
						}
						case BoxStyle::ThreeD: {
							DrawHelp::BoxScreen box;
							const auto p1 = camera->WorldToScreenPoint({ point.x - radius, point.y, point.z - radius }, II::Camera::Eye::Mono);
							if (p1.z < 0 && (p1.y > static_cast<float>(windowHeight) && p1.y < 0) && (p1.x < static_cast<float>(windowWidth) && p1.x > 0)) continue;
							box.lowerTopLeft     = DrawHelp::BoxScreen::ToVec2(p1);
							const auto p2 = camera->WorldToScreenPoint({ point.x - radius, point.y, point.z + radius }, II::Camera::Eye::Mono);
							if (p2.z < 0 && (p2.y > static_cast<float>(windowHeight) && p2.y < 0) && (p2.x < static_cast<float>(windowWidth) && p2.x > 0)) continue;
							box.lowerTopRight    = DrawHelp::BoxScreen::ToVec2(p2);
							const auto p3 = camera->WorldToScreenPoint({ point.x + radius, point.y, point.z - radius }, II::Camera::Eye::Mono);
							if (p3.z < 0 && (p3.y > static_cast<float>(windowHeight) && p3.y < 0) && (p3.x < static_cast<float>(windowWidth) && p3.x > 0)) continue;
							box.lowerBottomLeft  = DrawHelp::BoxScreen::ToVec2(p3);
							const auto p4 = camera->WorldToScreenPoint({ point.x + radius, point.y, point.z + radius }, II::Camera::Eye::Mono);
							if (p4.z < 0 && (p4.y > static_cast<float>(windowHeight) && p4.y < 0) && (p4.x < static_cast<float>(windowWidth) && p4.x > 0)) continue;
							box.lowerBottomRight = DrawHelp::BoxScreen::ToVec2(p4);

							const auto p5 = camera->WorldToScreenPoint({ point.x - radius, point.y + height, point.z - radius }, II::Camera::Eye::Mono);
							if (p5.z < 0 && (p5.y > static_cast<float>(windowHeight) && p5.y < 0) && (p5.x < static_cast<float>(windowWidth) && p5.x > 0)) continue;
							box.upperTopLeft     = DrawHelp::BoxScreen::ToVec2(p5);
							const auto p6 = camera->WorldToScreenPoint({ point.x - radius, point.y + height, point.z + radius }, II::Camera::Eye::Mono);
							if (p6.z < 0 && (p6.y > static_cast<float>(windowHeight) && p6.y < 0) && (p6.x < static_cast<float>(windowWidth) && p6.x > 0)) continue;
							box.upperTopRight    = DrawHelp::BoxScreen::ToVec2(p6);
							const auto p7 = camera->WorldToScreenPoint({ point.x + radius, point.y + height, point.z - radius }, II::Camera::Eye::Mono);
							if (p7.z < 0 && (p7.y > static_cast<float>(windowHeight) && p7.y < 0) && (p7.x < static_cast<float>(windowWidth) && p7.x > 0)) continue;
							box.upperBottomLeft  = DrawHelp::BoxScreen::ToVec2(p7);
							const auto p8 = camera->WorldToScreenPoint({ point.x + radius, point.y + height, point.z + radius }, II::Camera::Eye::Mono);
							if (p8.z < 0 && (p8.y > static_cast<float>(windowHeight) && p8.y < 0) && (p8.x < static_cast<float>(windowWidth) && p8.x > 0)) continue;
							box.upperBottomRight = DrawHelp::BoxScreen::ToVec2(p8);

							box.lowerTopLeft.y     = static_cast<float>(windowHeight) - box.lowerTopLeft.y;
							box.lowerTopRight.y    = static_cast<float>(windowHeight) - box.lowerTopRight.y;
							box.lowerBottomLeft.y  = static_cast<float>(windowHeight) - box.lowerBottomLeft.y;
							box.lowerBottomRight.y = static_cast<float>(windowHeight) - box.lowerBottomRight.y;
							box.upperBottomLeft.y  = static_cast<float>(windowHeight) - box.upperBottomLeft.y;
							box.upperBottomRight.y = static_cast<float>(windowHeight) - box.upperBottomRight.y;
							box.upperTopLeft.y     = static_cast<float>(windowHeight) - box.upperTopLeft.y;
							box.upperTopRight.y    = static_cast<float>(windowHeight) - box.upperTopRight.y;

							DrawHelp::DrawQuadLines(box.lowerBottomLeft, box.lowerTopLeft, box.lowerTopRight, box.lowerBottomRight, DrawHelp::GetTeamColor(team, 200));
							DrawHelp::DrawQuadLines(box.upperBottomLeft, box.upperTopLeft, box.upperTopRight, box.upperBottomRight, DrawHelp::GetTeamColor(team, 200));

							bg->AddLine(box.lowerBottomLeft, box.upperBottomLeft, DrawHelp::GetTeamColor(team, 200));
							bg->AddLine(box.lowerTopLeft, box.upperTopLeft, DrawHelp::GetTeamColor(team, 200));
							bg->AddLine(box.lowerTopRight, box.upperTopRight, DrawHelp::GetTeamColor(team, 200));
							bg->AddLine(box.lowerBottomRight, box.upperBottomRight, DrawHelp::GetTeamColor(team, 200));
							break;
						}
						default: break;
						}
					}

					if (setting.line) {
						const auto p1 = camera->WorldToScreenPoint({ point.x, point.y + height, point.z }, II::Camera::Eye::Mono);
						if (p1.z < 0) continue;
						auto xy = DrawHelp::BoxScreen::ToVec2(p1);
						xy.y = static_cast<float>(windowHeight) - xy.y;
						bg->AddLine({ static_cast<float>(windowWidth) / 2, 0 }, xy, DrawHelp::GetTeamColor(team, 200), 1);
					}

					if (setting.bone) {
						static I::Class* pAnimator;
						if (!pAnimator) pAnimator = I::Get("UnityEngine.AnimationModule.dll")->Get("Animator");
						if (pAnimator) {
							try {
								if (auto animator = player->roleControl->animatorControl->animator) {
									auto head      = animator->GetBoneTransform(II::Animator::HumanBodyBones::Head);
									auto chest     = animator->GetBoneTransform(II::Animator::HumanBodyBones::Chest);
									auto hips      = animator->GetBoneTransform(II::Animator::HumanBodyBones::Hips);
									auto leftArmU  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperArm);
									auto rightArmU = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperArm);
									auto leftArmL  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerArm);
									auto rightArmL = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerArm);
									auto leftLegU  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperLeg);
									auto rightLegU = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperLeg);
									auto leftLegL  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerLeg);
									auto rightLegL = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerLeg);

									const auto p1  = camera->WorldToScreenPoint(head->GetPosition(), II::Camera::Eye::Mono);
									const auto p2  = camera->WorldToScreenPoint(chest->GetPosition(), II::Camera::Eye::Mono);
									const auto p3  = camera->WorldToScreenPoint(hips->GetPosition(), II::Camera::Eye::Mono);
									const auto p4  = camera->WorldToScreenPoint(leftArmU->GetPosition(), II::Camera::Eye::Mono);
									const auto p5  = camera->WorldToScreenPoint(rightArmU->GetPosition(), II::Camera::Eye::Mono);
									const auto p6  = camera->WorldToScreenPoint(leftArmL->GetPosition(), II::Camera::Eye::Mono);
									const auto p7  = camera->WorldToScreenPoint(rightArmL->GetPosition(), II::Camera::Eye::Mono);
									const auto p8  = camera->WorldToScreenPoint(leftLegU->GetPosition(), II::Camera::Eye::Mono);
									const auto p9  = camera->WorldToScreenPoint(rightLegU->GetPosition(), II::Camera::Eye::Mono);
									const auto p10 = camera->WorldToScreenPoint(leftLegL->GetPosition(), II::Camera::Eye::Mono);
									const auto p11 = camera->WorldToScreenPoint(rightLegL->GetPosition(), II::Camera::Eye::Mono);

									#define B(x) x.z > -1
									if (B(p1) && B(p2) && B(p3) && B(p4) && B(p5) && B(p6) && B(p7) && B(p8) && B(p9) && B(p10) && B(p11)) {
										auto pHead      = DrawHelp::BoxScreen::ToVec2(p1);
										auto pChest     = DrawHelp::BoxScreen::ToVec2(p2);
										auto pHips      = DrawHelp::BoxScreen::ToVec2(p3);
										auto pLeftArmU  = DrawHelp::BoxScreen::ToVec2(p4);
										auto pRightArmU = DrawHelp::BoxScreen::ToVec2(p5);
										auto pLeftArmL  = DrawHelp::BoxScreen::ToVec2(p6);
										auto pRightArmL = DrawHelp::BoxScreen::ToVec2(p7);
										auto pLeftLegU  = DrawHelp::BoxScreen::ToVec2(p8);
										auto pRightLegU = DrawHelp::BoxScreen::ToVec2(p9);
										auto pLeftLegL  = DrawHelp::BoxScreen::ToVec2(p10);
										auto pRightLegL = DrawHelp::BoxScreen::ToVec2(p11);

										#define S(x) x.y = static_cast<float>(windowHeight) - x.y
										S(pHead);
										S(pChest);
										S(pHips);
										S(pLeftArmU);
										S(pRightArmU);
										S(pLeftArmL);
										S(pRightArmL);
										S(pLeftLegU);
										S(pRightLegU);
										S(pLeftLegL);
										S(pRightLegL);

										bg->AddLine(pHead, pChest, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pChest, pLeftArmU, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pChest, pRightArmU, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pLeftArmU, pLeftArmL, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pRightArmU, pRightArmL, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pChest, pHips, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pHips, pLeftLegU, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pHips, pRightLegU, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pLeftLegU, pLeftLegL, DrawHelp::GetTeamColor(team, 200), 1);
										bg->AddLine(pRightLegU, pRightLegL, DrawHelp::GetTeamColor(team, 200), 1);
									}
								}
							} catch (...) {
								ERROR()
							}
							
						}
					}

					if (setting.info) {
						auto p1 = camera->WorldToScreenPoint({ point.x, point.y + height, point.z }, II::Camera::Eye::Mono);
						if (p1.z < 0) continue;
						p1.y = static_cast<float>(windowHeight) - p1.y;
						auto xy = DrawHelp::BoxScreen::ToVec2(p1);
						bg->AddRectFilled({ xy.x - 100.0f, xy.y - 5.0f }, { xy.x + 100.0f, xy.y - 30.0f }, DrawHelp::GetTeamColor(team, 200), 0);
						DrawTextWithOutline(bg, { xy.x - 97.0f, xy.y - 27.0f }, std::format("{} | {}", team, player->roleLogic->name).c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
					}
				}
			}
			catch (...) {
				ERROR()
			}
		}
	}
}
auto PlayerEsp::Render() -> void {
	ImGui::Checkbox("启用", &enable);
	if (enable && ImGui::BeginTable("PlayerEsp", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 4))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"启用"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("方框ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.box);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("方框");
		ImGui::PopID();

		ImGui::PushID("信息ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.info);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("信息");
		ImGui::PopID();

		ImGui::PushID("射线ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.line);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("射线");
		ImGui::PopID();

		ImGui::PushID("骨骼ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.bone);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("骨骼");
		ImGui::PopID();

		ImGui::PushID("样式ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) {
			ImGui::RadioButton("平面", reinterpret_cast<int*>(&setting.boxStyle), 0);
			ImGui::SameLine();
			ImGui::RadioButton("立体", reinterpret_cast<int*>(&setting.boxStyle), 1);
		}
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("样式");
		ImGui::PopID();

		ImGui::EndTable();
	}
}
auto PlayerEsp::Update() -> void { Feature::Update(); }
auto PlayerEsp::Save(nlohmann::json& json) -> void { Feature::Save(json); }
auto PlayerEsp::Load(nlohmann::json& json) -> void { Feature::Load(json); }
auto PlayerEsp::GetInstance() -> PlayerEsp* {
	static PlayerEsp player;
	return &player;
}
