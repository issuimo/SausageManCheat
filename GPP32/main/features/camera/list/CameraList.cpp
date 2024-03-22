#include "CameraList.h"

#include "../../../GameDefine/Camera/CameraMove.hpp"

auto CameraList::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ "相机", "列表", true, false, false }; }
void CameraList::Draw() { Feature::Draw(); }
void CameraList::Render() {
	try {
		ImGui::Text(std::format("Main: {} | Current: {}", static_cast<void*>(II::Camera::GetMain()), static_cast<void*>(II::Camera::GetCurrent())).c_str());
	} catch (...) {
			ERROR("")
	}
	if (ImGui::BeginTable("CameraList", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Z缓冲", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& camera : II::Camera::GetAllCamera()) {
			ImGui::TableNextRow();
			ImGui::PushID(camera);
			try {
				if (ImGui::TableSetColumnIndex(0)) ImGui::Text(std::format("{:X}", reinterpret_cast<std::uint64_t>(camera)).c_str());
				if (ImGui::TableSetColumnIndex(1)) ImGui::Text(std::format("{}", camera->GetDepth()).c_str());
			}
			catch (...) {
				ERROR("")
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	if (ImGui::BeginTable("CameraList2", 1,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		if (ImGui::TableSetColumnIndex(0)) ImGui::Text(std::format("{:X}", reinterpret_cast<std::uint64_t>(CameraMove::main)).c_str());

		ImGui::EndTable();
	}
}
void CameraList::Update() { Feature::Update(); }
void CameraList::Save(nlohmann::json& json) { Feature::Save(json); }
void CameraList::Load(nlohmann::json& json) { Feature::Load(json); }
auto CameraList::GetInstance() -> CameraList* {
	static CameraList camera;
	return &camera;
}
