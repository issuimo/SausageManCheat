#include "PlayerEsp.h"
#include "../../../GameDefine/RoleLogic.h"

auto PlayerEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>("玩家"), reinterpret_cast<const char*>("透视"), true, false, true }; }
auto PlayerEsp::Draw() -> void {
	if (enable) {
		const auto bg = ImGui::GetBackgroundDrawList();
		std::lock_guard lock(RoleLogic::mutex);
		for (const auto& player : RoleLogic::vector) {
			try {
				if (player->isLocalRole && player->teamNum == RoleLogic::localPlayer->teamNum) continue;
				const auto camera = II::Camera::GetAllCamera()[0];
				auto       point  = player->nowPoint;
				const int  team   = player->teamNum;
				const int col = team % 5;
				if (auto sPoint = camera->WorldToScreenPoint(point, II::Camera::Eye::Mono); sPoint.z > -1) {
					sPoint.y = static_cast<float>(windowHeight) - sPoint.y;
					if (setting.box) {
						switch (setting.boxStyle) {
						case BoxStyle::Quad: {
							auto  xy = camera->WorldToScreenPoint({ point.x, point.y + 1.6f, point.z }, II::Camera::Eye::Mono);
							xy.y = static_cast<float>(windowHeight) - xy.y;
							const float w  = (sPoint.y - xy.y) / 3;

							ImVec2 p1 = DrawHelp::BoxScreen::ToVec2(xy), p2 = DrawHelp::BoxScreen::ToVec2(xy), p3 = DrawHelp::BoxScreen::ToVec2(sPoint), p4 =DrawHelp::BoxScreen::ToVec2(sPoint);
							p1.x -= w;
							p2.x += w;
							p3.x += w;
							p4.x -= w;

							DrawHelp::DrawQuadLines(p1, p2, p3, p4, DrawHelp::GetTeamCol(col, 200));
							break;
						}
						case BoxStyle::ThreeD: {
							DrawHelp::BoxScreen box;
							box.lowerTopLeft     = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x - 0.5f, point.y, point.z - 0.5f}, II::Camera::Eye::Mono));
							box.lowerTopRight    = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x - 0.5f, point.y, point.z + 0.5f}, II::Camera::Eye::Mono));
							box.lowerBottomLeft  = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x + 0.5f, point.y, point.z - 0.5f}, II::Camera::Eye::Mono));
							box.lowerBottomRight = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x + 0.5f, point.y, point.z + 0.5f}, II::Camera::Eye::Mono));

							box.upperTopLeft     = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x - 0.5f, point.y + 1.6f, point.z - 0.5f}, II::Camera::Eye::Mono));
							box.upperTopRight    = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x - 0.5f, point.y + 1.6f, point.z + 0.5f}, II::Camera::Eye::Mono));
							box.upperBottomLeft  = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x + 0.5f, point.y + 1.6f, point.z - 0.5f}, II::Camera::Eye::Mono));
							box.upperBottomRight = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({point.x + 0.5f, point.y + 1.6f, point.z + 0.5f}, II::Camera::Eye::Mono));

							box.lowerTopLeft.y     = static_cast<float>(windowHeight) - box.lowerTopLeft.y;
							box.lowerTopRight.y    = static_cast<float>(windowHeight) - box.lowerTopRight.y;
							box.lowerBottomLeft.y  = static_cast<float>(windowHeight) - box.lowerBottomLeft.y;
							box.lowerBottomRight.y = static_cast<float>(windowHeight) - box.lowerBottomRight.y;
							box.upperBottomLeft.y  = static_cast<float>(windowHeight) - box.upperBottomLeft.y;
							box.upperBottomRight.y = static_cast<float>(windowHeight) - box.upperBottomRight.y;
							box.upperTopLeft.y     = static_cast<float>(windowHeight) - box.upperTopLeft.y;
							box.upperTopRight.y    = static_cast<float>(windowHeight) - box.upperTopRight.y;

							DrawHelp::DrawQuadLines(box.lowerBottomLeft, box.lowerTopLeft, box.lowerTopRight, box.lowerBottomRight, DrawHelp::GetTeamCol(col, 200));
							DrawHelp::DrawQuadLines(box.upperBottomLeft, box.upperTopLeft, box.upperTopRight, box.upperBottomRight, DrawHelp::GetTeamCol(col, 200));

							bg->AddLine(box.lowerBottomLeft, box.upperBottomLeft, DrawHelp::GetTeamCol(col, 200));
							bg->AddLine(box.lowerTopLeft, box.upperTopLeft, DrawHelp::GetTeamCol(col, 200));
							bg->AddLine(box.lowerTopRight, box.upperTopRight, DrawHelp::GetTeamCol(col, 200));
							bg->AddLine(box.lowerBottomRight, box.upperBottomRight, DrawHelp::GetTeamCol(col, 200));
							break;
						}
						default: break;
						}
					}

					if (setting.info) {
						bg->AddRectFilled({ sPoint.x - 60.0f, sPoint.y + 15.0f }, { sPoint.x + 60.0f, sPoint.y + 55.0f }, DrawHelp::GetTeamCol(col, 200), 0);
						DrawTextWithOutline(bg, { sPoint.x - 57.0f, sPoint.y - 18.0f }, std::format("{} | {}", team, player->name).c_str(), ImColor{ 255,255,255 }, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
					}

					if (setting.line) {
						auto xy = DrawHelp::BoxScreen::ToVec2(camera->WorldToScreenPoint({ point.x, point.y + 1.6f, point.z }, II::Camera::Eye::Mono));
						xy.y = static_cast<float>(windowHeight) - xy.y;
						bg->AddLine({ static_cast<float>(windowWidth) / 2, 0 }, xy, DrawHelp::GetTeamCol(col, 200), 1);
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
