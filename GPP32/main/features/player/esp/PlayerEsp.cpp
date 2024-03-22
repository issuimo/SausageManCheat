#include "PlayerEsp.h"
#include "../../../GameDefine/Role/Role.h"

auto PlayerEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{"玩家", "透视", true, false, true}; }

auto PlayerEsp::Draw() -> void {
	if (enable) {
		const auto      bg = ImGui::GetBackgroundDrawList();
		std::lock_guard lock(Role::mutex);
        if (Role::localRole) {
            for (const auto &player : Role::vector) {
                [&] {
                    __try {
                        [&] {
                            if (player == Role::localRole || Role::localRole->roleLogic->TeamNum == player->roleLogic->TeamNum) return;
                            II::Vector3 point = player->roleLogic->NowPoint;
                            const int   team  = player->roleLogic->TeamNum;
                            II::Vector3 scale;
                            II::Vector3 pHead;
                            float       height;

                            scale  = player->GetTransform()->GetLocalScale();
                            pHead  = player->MyRoleControl->animatorControl->animator->GetBoneTransform(II::Animator::HumanBodyBones::Head)->GetPosition();
                            height = pHead.y - point.y + 0.6 * scale.y;

                            float radius  = 0.5 * scale.x;
                            float radiusZ = 0.5 * scale.z;

                            bool weak{false};

                            if (player->roleLogic->hp < 0 && player->roleLogic->WeakValue < 100) {
                                height  = 0.5;
                                radius  = 0.3;
                                radiusZ = 0.3;
                                weak    = true;
                            }

                            if (weak && !setting.weak) return;

                            float fovX = -player->MyRoleControl->y_camRot;

							II::Vector3 mPoint = Role::localRole->roleLogic->NowPoint;

                            if (auto sPoint = WorldToScreenPoint(point); sPoint.z > 0) {
                                if (setting.viewLook) {
                                    II::Vector3 topPoint1 = {point.x, point.y + height - 0.3f, point.z + radius};
                                    RotatePoint(topPoint1.x, topPoint1.z, point.x, point.z, fovX, topPoint1.x, topPoint1.z);
                                    II::Vector3 topPoint2 = {point.x, point.y + height - 0.3f, point.z};
                                    bg->AddLine(DrawHelp::BoxScreen::ToVec2(WorldToScreenPoint(topPoint1)), DrawHelp::BoxScreen::ToVec2(WorldToScreenPoint(topPoint2)), DrawHelp::GetTeamColor(team, 200));
                                    bg->AddCircleFilled(DrawHelp::BoxScreen::ToVec2(WorldToScreenPoint(topPoint1)), 3, DrawHelp::GetTeamColor(team, 200));
                                }

                                if (setting.box) {

                                    switch (setting.boxStyle) {
                                        case BoxStyle::Quad: {
                                            auto xy = WorldToScreenPoint({point.x, point.y + height, point.z});
                                            if (xy.z < 0) return;
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
                                            II::Vector3         p1Vec3 = {point.x - radius, point.y, point.z - radiusZ};
                                            RotatePoint(p1Vec3.x, p1Vec3.z, point.x, point.z, fovX, p1Vec3.x, p1Vec3.z);
                                            const auto p1 = WorldToScreenPoint(p1Vec3);
                                            if (p1.z < 0) return;
                                            box.lowerTopLeft = DrawHelp::BoxScreen::ToVec2(p1);

                                            II::Vector3 p2Vec3 = {point.x - radius, point.y, point.z + radiusZ};
                                            RotatePoint(p2Vec3.x, p2Vec3.z, point.x, point.z, fovX, p2Vec3.x, p2Vec3.z);
                                            const auto p2 = WorldToScreenPoint(p2Vec3);
                                            if (p2.z < 0) return;
                                            box.lowerTopRight = DrawHelp::BoxScreen::ToVec2(p2);

                                            II::Vector3 p3Vec3 = {point.x + radius, point.y, point.z - radiusZ};
                                            RotatePoint(p3Vec3.x, p3Vec3.z, point.x, point.z, fovX, p3Vec3.x, p3Vec3.z);
                                            const auto p3 = WorldToScreenPoint(p3Vec3);
                                            if (p3.z < 0) return;
                                            box.lowerBottomLeft = DrawHelp::BoxScreen::ToVec2(p3);

                                            II::Vector3 p4Vec3 = {point.x + radius, point.y, point.z + radiusZ};
                                            RotatePoint(p4Vec3.x, p4Vec3.z, point.x, point.z, fovX, p4Vec3.x, p4Vec3.z);
                                            const auto p4 = WorldToScreenPoint(p4Vec3);
                                            if (p4.z < 0) return;
                                            box.lowerBottomRight = DrawHelp::BoxScreen::ToVec2(p4);


                                            II::Vector3 p5Vec3 = {point.x - radius, point.y + height, point.z - radiusZ};
                                            RotatePoint(p5Vec3.x, p5Vec3.z, point.x, point.z, fovX, p5Vec3.x, p5Vec3.z);
                                            const auto p5 = WorldToScreenPoint(p5Vec3);
                                            if (p5.z < 0) return;
                                            box.upperTopLeft = DrawHelp::BoxScreen::ToVec2(p5);

                                            II::Vector3 p6Vec3 = {point.x - radius, point.y + height, point.z + radiusZ};
                                            RotatePoint(p6Vec3.x, p6Vec3.z, point.x, point.z, fovX, p6Vec3.x, p6Vec3.z);
                                            const auto p6 = WorldToScreenPoint(p6Vec3);
                                            if (p6.z < 0) return;
                                            box.upperTopRight = DrawHelp::BoxScreen::ToVec2(p6);

                                            II::Vector3 p7Vec3 = {point.x + radius, point.y + height, point.z - radiusZ};
                                            RotatePoint(p7Vec3.x, p7Vec3.z, point.x, point.z, fovX, p7Vec3.x, p7Vec3.z);
                                            const auto p7 = WorldToScreenPoint(p7Vec3);
                                            if (p7.z < 0) return;
                                            box.upperBottomLeft = DrawHelp::BoxScreen::ToVec2(p7);

                                            II::Vector3 p8Vec3 = {point.x + radius, point.y + height, point.z + radiusZ};
                                            RotatePoint(p8Vec3.x, p8Vec3.z, point.x, point.z, fovX, p8Vec3.x, p8Vec3.z);
                                            const auto p8 = WorldToScreenPoint(p8Vec3);
                                            if (p8.z < 0) return;
                                            box.upperBottomRight = DrawHelp::BoxScreen::ToVec2(p8);


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
                                    const auto p1 = WorldToScreenPoint({point.x, point.y + height, point.z});
                                    if (p1.z < 0) return;
                                    auto xy = DrawHelp::BoxScreen::ToVec2(p1);
                                    bg->AddLine({static_cast<float>(windowWidth) / 2, 0}, xy, DrawHelp::GetTeamColor(team, 200), 1);
                                }

                                if (setting.bone && !weak) {
                                    if (auto animator = player->MyRoleControl->animatorControl->animator) {
                                        II::Vector3 p1;
                                        II::Vector3 p2;
                                        II::Vector3 p3;
                                        II::Vector3 p4;
                                        II::Vector3 p5;
                                        II::Vector3 p6;
                                        II::Vector3 p7;
                                        II::Vector3 p8;
                                        II::Vector3 p9;
                                        II::Vector3 p10;
                                        II::Vector3 p11;
                                        II::Vector3 p12;
                                        II::Vector3 p13;
                                        II::Vector3 p14;
                                        II::Vector3 p15;

                                        const auto head      = animator->GetBoneTransform(II::Animator::HumanBodyBones::Head);
                                        const auto chest     = animator->GetBoneTransform(II::Animator::HumanBodyBones::Chest);
                                        const auto hips      = animator->GetBoneTransform(II::Animator::HumanBodyBones::Hips);
                                        const auto leftArmU  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperArm);
                                        const auto rightArmU = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperArm);
                                        const auto leftArmL  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerArm);
                                        const auto rightArmL = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerArm);
                                        const auto leftLegU  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftUpperLeg);
                                        const auto rightLegU = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightUpperLeg);
                                        const auto leftLegL  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftLowerLeg);
                                        const auto rightLegL = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightLowerLeg);
                                        const auto rightHand = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightHand);
                                        const auto leftHand  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftHand);
                                        const auto rightFoot = animator->GetBoneTransform(II::Animator::HumanBodyBones::RightFoot);
                                        const auto leftFoot  = animator->GetBoneTransform(II::Animator::HumanBodyBones::LeftFoot);

                                        p1  = WorldToScreenPoint(head->GetPosition());
                                        p2  = WorldToScreenPoint(chest->GetPosition());
                                        p3  = WorldToScreenPoint(hips->GetPosition());
                                        p4  = WorldToScreenPoint(leftArmU->GetPosition());
                                        p5  = WorldToScreenPoint(rightArmU->GetPosition());
                                        p6  = WorldToScreenPoint(leftArmL->GetPosition());
                                        p7  = WorldToScreenPoint(rightArmL->GetPosition());
                                        p8  = WorldToScreenPoint(leftLegU->GetPosition());
                                        p9  = WorldToScreenPoint(rightLegU->GetPosition());
                                        p10 = WorldToScreenPoint(leftLegL->GetPosition());
                                        p11 = WorldToScreenPoint(rightLegL->GetPosition());
                                        p12 = WorldToScreenPoint(rightHand->GetPosition());
                                        p13 = WorldToScreenPoint(leftHand->GetPosition());
                                        p14 = WorldToScreenPoint(rightFoot->GetPosition());
                                        p15 = WorldToScreenPoint(leftFoot->GetPosition());

#define B(x) x.z > -1
                                        if (B(p1) && B(p2) && B(p3) && B(p4) && B(p5) && B(p6) && B(p7) && B(p8) && B(p9) && B(p10) && B(p11) && B(p12) && B(p13) && B(p14) && B(p15)) {
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
                                            auto pRightHand = DrawHelp::BoxScreen::ToVec2(p12);
                                            auto pLeftHand  = DrawHelp::BoxScreen::ToVec2(p13);
                                            auto pRightFoot = DrawHelp::BoxScreen::ToVec2(p14);
                                            auto pLeftFoot  = DrawHelp::BoxScreen::ToVec2(p15);

                                            bg->AddLine(pHead, pChest, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pChest, pLeftArmU, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pChest, pRightArmU, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pLeftArmU, pLeftArmL, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pRightArmU, pRightArmL, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pLeftArmL, pLeftHand, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pRightArmL, pRightHand, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pChest, pHips, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pHips, pLeftLegU, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pHips, pRightLegU, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pLeftLegU, pLeftLegL, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pRightLegU, pRightLegL, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pLeftLegL, pLeftFoot, DrawHelp::GetTeamColor(team, 255), 1);
                                            bg->AddLine(pRightLegL, pRightFoot, DrawHelp::GetTeamColor(team, 255), 1);
                                        }
                                    }
                                }

                                if (setting.info) {
                                    auto p1 = WorldToScreenPoint({point.x, point.y + height, point.z});
                                    if (p1.z < 0) return;
                                    auto xy = DrawHelp::BoxScreen::ToVec2(p1);
                                    bg->AddRectFilled({xy.x - 100.0f, xy.y}, {xy.x + 100.0f, xy.y - 30.0f}, DrawHelp::GetTeamColor(team, 150), 0);
                                    bg->AddRectFilled({xy.x - 100.0f, xy.y}, {xy.x - 70.0f, xy.y - 30.0f}, DrawHelp::GetTeamColor(team, 255), 0);
                                    bg->AddLine({xy.x - 100.0f, xy.y}, {(xy.x + 100.0f), xy.y}, ImColor{0, 0, 0}, 3);
                                    bg->AddLine({xy.x - 100.0f, xy.y}, {xy.x - 100.0f + 200 * (player->roleLogic->hp / player->roleLogic->maxHp), xy.y}, ImColor{255, 255, 255}, 3);

                                    std::string playerName = team ? player->roleLogic->NickName->ToString() : "[人机] " + player->roleLogic->NickName->ToString();
                                    DrawTextWithOutline(bg, {xy.x - 95.0f, xy.y - 25.0f}, std::format("{:02} | {}", team, playerName).c_str(), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});

                                    if (weak) DrawTextWithOutline(bg, {xy.x - 95.0f, xy.y + 10.0f}, reinterpret_cast<const char *>(u8"倒地"), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});

                                    if (setting.part) {
                                        std::string palyerWeaponName = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->MyPickItemData->itemName->ToString() : "空手";
                                        int         bullet           = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->BulletNum : 0;
                                        DrawTextWithOutline(bg, {xy.x - 100.0f, xy.y - 45.0f}, std::format("{}m | {} kills | {}:{}", static_cast<int>(sqrt(pow(point.x - mPoint.x, 2) + pow(point.y - mPoint.y, 2) + pow(point.z - mPoint.z, 2))), player->roleLogic->KillRoleNum, bullet, palyerWeaponName).c_str(), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                                    } else {
                                        std::string palyerWeaponName = player->roleLogic->UserWeapon ? player->roleLogic->UserWeapon->MyPickItemData->itemName->ToString() : "空手";
                                        DrawTextWithOutline(bg, {xy.x - 100.0f, xy.y - 45.0f}, std::format("{}m | {} kills", static_cast<int>(sqrt(pow(point.x - mPoint.x, 2) + pow(point.y - mPoint.y, 2) + pow(point.z - mPoint.z, 2))), player->roleLogic->KillRoleNum).c_str(), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                                    }

                                    if (setting.part) {
                                        const std::string headPart = player->roleLogic->HeadEquipPart ? player->roleLogic->HeadEquipPart->MyPickItemData->itemName->ToString() : "无";
                                        const std::string bodyPart = player->roleLogic->BodyEquipPart ? player->roleLogic->BodyEquipPart->MyPickItemData->itemName->ToString() : "无";
                                        const std::string packPart = player->roleLogic->PackEquip ? player->roleLogic->PackEquip->MyPickItemData->itemName->ToString() : "无";
                                        DrawTextWithOutline(bg, {xy.x + 101.0f, xy.y}, headPart.c_str(), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                                        DrawTextWithOutline(bg, {xy.x + 101.0f, xy.y - 15}, bodyPart.c_str(), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                                        DrawTextWithOutline(bg, {xy.x + 101.0f, xy.y - 30}, packPart.c_str(), ImColor{255, 255, 255}, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                                    }
                                }

                            }
                        }();
                    } __except (EXCEPTION_EXECUTE_HANDLER) {
                        [] { ERROR("PlayerEsp Error (for) (except)"); }();
                    }
                }();
            }
        }
	}
}

auto PlayerEsp::Render() -> void {
	ImGui::Checkbox("启用", &enable);
	if (enable && ImGui::BeginTable("PlayerEsp", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("启用", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
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

		ImGui::PushID("装备ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.part);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("装备");
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

		ImGui::PushID("倒地ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.weak);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("倒地");
		ImGui::PopID();

		ImGui::PushID("朝向ch");
		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &setting.viewLook);
		if (ImGui::TableSetColumnIndex(1)) ImGui::Text("朝向");
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

auto PlayerEsp::Save(nlohmann::json& json) -> void {
	json["PlayerESP"]["enable"]   = enable;
	json["PlayerESP"]["box"]      = setting.box;
	json["PlayerESP"]["info"]     = setting.info;
	json["PlayerESP"]["line"]     = setting.line;
	json["PlayerESP"]["bone"]     = setting.bone;
	json["PlayerESP"]["part"]     = setting.part;
	json["PlayerESP"]["boxStyle"] = setting.boxStyle;
	json["PlayerESP"]["weak"] = setting.weak;
	json["PlayerESP"]["viewLook"]     = setting.viewLook;
}

auto PlayerEsp::Load(nlohmann::json& json) -> void {
	if (json.contains("PlayerESP")) {
		if (json["PlayerESP"].contains("enable")) enable = json["PlayerESP"]["enable"];
		if (json["PlayerESP"].contains("box")) setting.box = json["PlayerESP"]["box"];
		if (json["PlayerESP"].contains("info")) setting.info = json["PlayerESP"]["info"];
		if (json["PlayerESP"].contains("line")) setting.line = json["PlayerESP"]["line"];
		if (json["PlayerESP"].contains("bone")) setting.bone = json["PlayerESP"]["bone"];
		if (json["PlayerESP"].contains("part")) setting.part = json["PlayerESP"]["part"];
		if (json["PlayerESP"].contains("boxStyle")) setting.boxStyle = json["PlayerESP"]["boxStyle"];
		if (json["PlayerESP"].contains("weak")) setting.weak = json["PlayerESP"]["weak"];
		if (json["PlayerESP"].contains("viewLook")) setting.viewLook = json["PlayerESP"]["viewLook"];
	}
}

auto PlayerEsp::GetInstance() -> PlayerEsp* {
	static PlayerEsp player;
	return &player;
}
