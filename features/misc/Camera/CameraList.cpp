#include "CameraList.h"
cheat::feature::CameraList* cheat::feature::CameraList::GetInstance() {
	static CameraList cam;
	return &cam;
}
const cheat::FeatureGUIInfo& cheat::feature::CameraList::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "相机列表", "调试", true };
	return info;
}
void cheat::feature::CameraList::DrawMain() { 
	if (ImGui::BeginTable("CameraList", 1,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn(reinterpret_cast<const char*>(u8"地址"), ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& cam : II::Camera::GetAllCamera()) {
			ImGui::TableNextRow();
			ImGui::PushID(&cam);
			try {
				if (ImGui::TableSetColumnIndex(0)) ImGui::Text("%p", cam);
			}
			catch (const SEH_exception& e) {
				LOG_ERROR("0x%p %s", e.code(), e.what());
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
bool cheat::feature::CameraList::NeedStatusDraw() const { return Feature::NeedStatusDraw(); }
void cheat::feature::CameraList::DrawStatus() { Feature::DrawStatus(); }
bool cheat::feature::CameraList::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::CameraList::DrawInfo() { Feature::DrawInfo(); }
void cheat::feature::CameraList::DrawExternal() { Feature::DrawExternal(); }
cheat::feature::CameraList::CameraList() {  }
