#include "PlayerEsp.h"
#include "../../../GameDefine/Role/Role.h"

auto PlayerEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>(u8"玩家"), reinterpret_cast<const char*>(u8"透视"), true, false, true }; }
auto PlayerEsp::Draw() -> void {
	if (enable) {
		const auto bg = ImGui::GetBackgroundDrawList();
		std::lock_guard lock(Role::mutex);
		try {
			if (Role::localRole) {
				const auto camera = II::Camera::GetAllCamera()[0];
				II::Vector3 mPoint;
				if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(RoleLogic))) {
					mPoint = Role::localRole->roleLogic->NowPoint;
				}

				for (const auto& player : Role::vector) {
					try {
						if (player == Role::localRole ||
							IsBadReadPtr(Role::localRole, sizeof(RoleLogic)) ||
							IsBadReadPtr(player, sizeof(Role)) ||
							IsBadReadPtr(player->MyRoleControl, sizeof(RoleControl)) ||
							IsBadReadPtr(player->roleLogic, sizeof(RoleLogic)) ||
							Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) continue;

						auto       point = player->roleLogic->NowPoint;
						const int  team = player->roleLogic->TeamNum;
						auto scale = player->GetTransform()->GetLocalScale();
						auto pHead = player->MyRoleControl->animatorControl->animator->GetBoneTransform(II::Animator::HumanBodyBones::Head);
						const float height = (pHead->GetPosition().y - point.y) + 0.6 * scale.y;
						const float radius = 0.5 * scale.x;
						const float radiusZ = 0.5 * scale.z;

						if (auto sPoint = camera->WorldToScreenPoint(point, II::Camera::Eye::Mono);
							sPoint.z > 0) {
							sPoint.y = static_cast<float>(windowHeight) - sPoint.y;
							if (setting.box) {
								switch (setting.boxStyle) {
								case BoxStyle::Quad: {
									auto  xy = camera->WorldToScreenPoint({ point.x, point.y + height, point.z }, II::Camera::Eye::Mono);
									if (xy.z < 0) continue;
									xy.y = static_cast<float>(windowHeight) - xy.y;
									const float w = (sPoint.y - xy.y) / 3;

									ImVec2 p1 = DrawHelp::BoxScreen::ToVec2(xy), p2 = DrawHelp::BoxScreen::ToVec2(xy), p3 = DrawHelp::BoxScreen::ToVec2(sPoint), p4 = DrawHelp::BoxScreen::ToVec2(sPoint);
									p1.x -= w;
									p2.x += w;
									p3.x += w;
									p4.x -= w;

									DrawHelp::DrawQuadLines(p1, p2, p3, p4, DrawHelp::GetTeamColor(team, 200));
									break;
								}
								case BoxStyle::ThreeD: {
									DrawHelp::BoxScreen box;
									const auto p1 = camera->WorldToScreenPoint({ point.x - radius, point.y, point.z - radiusZ }, II::Camera::Eye::Mono);
									if (p1.z < 0) continue;
									box.lowerTopLeft = DrawHelp::BoxScreen::ToVec2(p1);
									const auto p2 = camera->WorldToScreenPoint({ point.x - radius, point.y, point.z + radiusZ }, II::Camera::Eye::Mono);
									if (p2.z < 0) continue;
									box.lowerTopRight = DrawHelp::BoxScreen::ToVec2(p2);
									const auto p3 = camera->WorldToScreenPoint({ point.x + radius, point.y, point.z - radiusZ }, II::Camera::Eye::Mono);
									if (p3.z < 0) continue;
									box.lowerBottomLeft = DrawHelp::BoxScreen::ToVec2(p3);
									const auto p4 = camera->WorldToScreenPoint({ point.x + radius, point.y, point.z + radiusZ }, II::Camera::Eye::Mono);
									if (p4.z < 0) continue;
									box.lowerBottomRight = DrawHelp::BoxScreen::ToVec2(p4);

									const auto p5 = camera->WorldToScreenPoint({ point.x - radius, point.y + height, point.z - radiusZ }, II::Camera::Eye::Mono);
									if (p5.z < 0) continue;
									box.upperTopLeft = DrawHelp::BoxScreen::ToVec2(p5);
									const auto p6 = camera->WorldToScreenPoint({ point.x - radius, point.y + height, point.z + radiusZ }, II::Camera::Eye::Mono);
									if (p6.z < 0) continue;
									box.upperTopRight = DrawHelp::BoxScreen::ToVec2(p6);
									const auto p7 = camera->WorldToScreenPoint({ point.x + radius, point.y + height, point.z - radiusZ }, II::Camera::Eye::Mono);
									if (p7.z < 0) continue;
									box.upperBottomLeft = DrawHelp::BoxScreen::ToVec2(p7);
									const auto p8 = camera->WorldToScreenPoint({ point.x + radius, point.y + height, point.z + radiusZ }, II::Camera::Eye::Mono);
									if (p8.z < 0) continue;
									box.upperBottomRight = DrawHelp::BoxScreen::ToVec2(p8);

									box.lowerTopLeft.y = static_cast<float>(windowHeight) - box.lowerTopLeft.y;
									box.lowerTopRight.y = static_cast<float>(windowHeight) - box.lowerTopRight.y;
									box.lowerBottomLeft.y = static_cast<float>(windowHeight) - box.lowerBottomLeft.y;
									box.lowerBottomRight.y = static_cast<float>(windowHeight) - box.lowerBottomRight.y;
									box.upperBottomLeft.y = static_cast<float>(windowHeight) - box.upperBottomLeft.y;
									box.upperBottomRight.y = static_cast<float>(windowHeight) - box.upperBottomRight.y;
									box.upperTopLeft.y = static_cast<float>(windowHeight) - box.upperTopLeft.y;
									box.upperTopRight.y = static_cast<float>(windowHeight) - box.upperTopRight.y;

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
								try {
									if (auto animator = player->MyRoleControl->animatorControl->animator) {
										auto head = animator->GetBoneTransform(II::Animator::HumanBodyBones::Head);
										auto chest = animator->GetBoneTransform(II::Animator::HumanBodyBones::Chest);
										auto hips = animator->GetBoneTransform(II::Animator::HumanBodyBones::Hips);
										auto leftArmU = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperArm);
										auto rightArmU = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperArm);
										auto leftArmL = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerArm);
										auto rightArmL = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerArm);
										auto leftLegU = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperLeg);
										auto rightLegU = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperLeg);
										auto leftLegL = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerLeg);
										auto rightLegL = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerLeg);
										auto rightHand = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightHand);
										auto leftHand = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftHand);
										auto rightFoot = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightFoot);
										auto leftFoot = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftFoot);

										const auto p1 = camera->WorldToScreenPoint(head->GetPosition(), II::Camera::Eye::Mono);
										const auto p2 = camera->WorldToScreenPoint(chest->GetPosition(), II::Camera::Eye::Mono);
										const auto p3 = camera->WorldToScreenPoint(hips->GetPosition(), II::Camera::Eye::Mono);
										const auto p4 = camera->WorldToScreenPoint(leftArmU->GetPosition(), II::Camera::Eye::Mono);
										const auto p5 = camera->WorldToScreenPoint(rightArmU->GetPosition(), II::Camera::Eye::Mono);
										const auto p6 = camera->WorldToScreenPoint(leftArmL->GetPosition(), II::Camera::Eye::Mono);
										const auto p7 = camera->WorldToScreenPoint(rightArmL->GetPosition(), II::Camera::Eye::Mono);
										const auto p8 = camera->WorldToScreenPoint(leftLegU->GetPosition(), II::Camera::Eye::Mono);
										const auto p9 = camera->WorldToScreenPoint(rightLegU->GetPosition(), II::Camera::Eye::Mono);
										const auto p10 = camera->WorldToScreenPoint(leftLegL->GetPosition(), II::Camera::Eye::Mono);
										const auto p11 = camera->WorldToScreenPoint(rightLegL->GetPosition(), II::Camera::Eye::Mono);
										const auto p12 = camera->WorldToScreenPoint(rightHand->GetPosition(), II::Camera::Eye::Mono);
										const auto p13 = camera->WorldToScreenPoint(leftHand->GetPosition(), II::Camera::Eye::Mono);
										const auto p14 = camera->WorldToScreenPoint(rightFoot->GetPosition(), II::Camera::Eye::Mono);
										const auto p15 = camera->WorldToScreenPoint(leftFoot->GetPosition(), II::Camera::Eye::Mono);

#define B(x) x.z > -1
										if (B(p1) && B(p2) && B(p3) && B(p4) && B(p5) && B(p6) && B(p7) && B(p8) && B(p9) && B(p10) && B(p11) && B(p12) && B(p13) && B(p14) && B(p15)) {
											auto pHead = DrawHelp::BoxScreen::ToVec2(p1);
											auto pChest = DrawHelp::BoxScreen::ToVec2(p2);
											auto pHips = DrawHelp::BoxScreen::ToVec2(p3);
											auto pLeftArmU = DrawHelp::BoxScreen::ToVec2(p4);
											auto pRightArmU = DrawHelp::BoxScreen::ToVec2(p5);
											auto pLeftArmL = DrawHelp::BoxScreen::ToVec2(p6);
											auto pRightArmL = DrawHelp::BoxScreen::ToVec2(p7);
											auto pLeftLegU = DrawHelp::BoxScreen::ToVec2(p8);
											auto pRightLegU = DrawHelp::BoxScreen::ToVec2(p9);
											auto pLeftLegL = DrawHelp::BoxScreen::ToVec2(p10);
											auto pRightLegL = DrawHelp::BoxScreen::ToVec2(p11);
											auto pRightHand = DrawHelp::BoxScreen::ToVec2(p12);
											auto pLeftHand = DrawHelp::BoxScreen::ToVec2(p13);
											auto pRightFoot = DrawHelp::BoxScreen::ToVec2(p14);
											auto pLeftFoot = DrawHelp::BoxScreen::ToVec2(p15);

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
											S(pRightHand);
											S(pLeftHand);
											S(pRightFoot);
											S(pLeftFoot);

											bg->AddLine(pHead, pChest, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pChest, pLeftArmU, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pChest, pRightArmU, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pLeftArmU, pLeftArmL, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pRightArmU, pRightArmL, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pLeftArmL, pLeftHand, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pRightArmL, pRightHand, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pChest, pHips, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pHips, pLeftLegU, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pHips, pRightLegU, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pLeftLegU, pLeftLegL, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pRightLegU, pRightLegL, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pLeftLegL, pLeftFoot, DrawHelp::GetTeamColor(team, 255), 2);
											bg->AddLine(pRightLegL, pRightFoot, DrawHelp::GetTeamColor(team, 255), 2);
										}
									}
								}
								catch (...) {
									ERROR("Animator->nullptr")
								}
							}

							if (setting.info) {
								auto p1 = camera->WorldToScreenPoint({ point.x, point.y + height, point.z }, II::Camera::Eye::Mono);
								if (p1.z < 0) continue;
								p1.y = static_cast<float>(windowHeight) - p1.y;
								auto xy = DrawHelp::BoxScreen::ToVec2(p1);
								bg->AddRectFilled({ xy.x - 100.0f, xy.y }, { xy.x + 100.0f, xy.y - 30.0f }, DrawHelp::GetTeamColor(team, 150), 0);
								bg->AddRectFilled({ xy.x - 100.0f, xy.y }, { xy.x - 70.0f, xy.y - 30.0f }, DrawHelp::GetTeamColor(team, 255), 0);
								bg->AddLine({ xy.x - 100.0f, xy.y }, { (xy.x + 100.0f), xy.y }, ImColor{ 0,0,0 }, 3);
								bg->AddLine({ xy.x - 100.0f, xy.y }, { (xy.x - 100.0f) + 200 * (player->roleLogic->hp / player->roleLogic->maxHp), xy.y }, ImColor{ 255,255,255 }, 3);

								std::string playerName = team ? player->roleLogic->NickName->ToString() : GbkToUtf8("[人机] ") + player->roleLogic->NickName->ToString();
								DrawTextWithOutline(bg, { xy.x - 95.0f, xy.y - 25.0f }, std::format("{:02} | {}", team, playerName).c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });

								if (setting.part) {
									std::string palyerWeaponName = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->MyPickItemData->itemName->ToString() : GbkToUtf8("空手");
									int bullet = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->BulletNum : 0;
									DrawTextWithOutline(bg, { xy.x - 100.0f, xy.y - 45.0f }, std::format("{}m | {} kills | {}:{}", (int)sqrt(pow(point.x - mPoint.x, 2) + pow(point.y - mPoint.y, 2) + pow(point.z - mPoint.z, 2)), player->roleLogic->KillRoleNum, bullet, palyerWeaponName).c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
								}
								else {
									std::string palyerWeaponName = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->MyPickItemData->itemName->ToString() : GbkToUtf8("空手");
									int bullet = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->BulletNum : 0;
									DrawTextWithOutline(bg, { xy.x - 100.0f, xy.y - 45.0f }, std::format("{}m | {} kills", (int)sqrt(pow(point.x - mPoint.x, 2) + pow(point.y - mPoint.y, 2) + pow(point.z - mPoint.z, 2)), player->roleLogic->KillRoleNum).c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
								}

								if (setting.part) {
									std::string headPart = player->roleLogic->HeadEquipPart ? player->roleLogic->HeadEquipPart->MyPickItemData->itemName->ToString() : GbkToUtf8("无");
									std::string bodyPart = player->roleLogic->BodyEquipPart ? player->roleLogic->BodyEquipPart->MyPickItemData->itemName->ToString() : GbkToUtf8("无");
									std::string packPart = player->roleLogic->PackEquip ? player->roleLogic->PackEquip->MyPickItemData->itemName->ToString() : GbkToUtf8("无");
									DrawTextWithOutline(bg, { xy.x + 101.0f, xy.y }, headPart.c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
									DrawTextWithOutline(bg, { xy.x + 101.0f, xy.y - 15 }, bodyPart.c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
									DrawTextWithOutline(bg, { xy.x + 101.0f, xy.y - 30 }, packPart.c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
								}
							}
						}
					}
					catch (...) {
						ERROR("PlayerEsp->nullptr")
					}
				}
			}

		} catch (...) {
			ERROR("PlayerEsp->top->nullptr")
		}
	}
}
auto PlayerEsp::Render() -> void {
	ImGui::Checkbox((const char*)u8"启用", &enable);
	if (enable && ImGui::BeginTable("PlayerEsp", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"启用"), ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"名称"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::PushID("方框ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.box);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"方框");
		ImGui::PopID();

		ImGui::PushID("信息ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.info);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"信息");
		ImGui::PopID();

		ImGui::PushID("装备ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.part);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"装备");
		ImGui::PopID();

		ImGui::PushID("射线ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.line);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"射线");
		ImGui::PopID();

		ImGui::PushID("骨骼ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.bone);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"骨骼");
		ImGui::PopID();

		ImGui::PushID("样式ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) {
			ImGui::RadioButton((const char*)u8"平面", reinterpret_cast<int*>(&setting.boxStyle), 0);
			ImGui::SameLine();
			ImGui::RadioButton((const char*)u8"立体", reinterpret_cast<int*>(&setting.boxStyle), 1);
		}
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text((const char*)u8"样式");
		ImGui::PopID();

		ImGui::EndTable();
	}
}
auto PlayerEsp::Update() -> void { Feature::Update(); }
auto PlayerEsp::Save(nlohmann::json& json) -> void {
	json["PlayerESP"]["enable"] = enable;
	json["PlayerESP"]["box"] = setting.box;
	json["PlayerESP"]["info"] = setting.info;
	json["PlayerESP"]["line"] = setting.line;
	json["PlayerESP"]["bone"] = setting.bone;
	json["PlayerESP"]["part"] = setting.part;
	json["PlayerESP"]["boxStyle"] = setting.boxStyle;
}
auto PlayerEsp::Load(nlohmann::json& json) -> void {
	if (json.contains("PlayerESP")) {
		if (json["PlayerESP"].contains("enable")) {
			enable = json["PlayerESP"]["enable"];
		}
		if (json["PlayerESP"].contains("box")) {
			setting.box = json["PlayerESP"]["box"];
		}
		if (json["PlayerESP"].contains("info")) {
			setting.info = json["PlayerESP"]["info"];
		}
		if (json["PlayerESP"].contains("line")) {
			setting.line = json["PlayerESP"]["line"];
		}
		if (json["PlayerESP"].contains("bone")) {
			setting.bone = json["PlayerESP"]["bone"];
		}
		if (json["PlayerESP"].contains("part")) {
			setting.part = json["PlayerESP"]["part"];
		}
		if (json["PlayerESP"].contains("boxStyle")) {
			setting.boxStyle = json["PlayerESP"]["boxStyle"];
		}
	}
}
auto PlayerEsp::GetInstance() -> PlayerEsp* {
	static PlayerEsp player;
	return &player;
}
