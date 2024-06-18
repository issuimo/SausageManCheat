#include "CarList.h"
#include "../../../CLASS/Car.h"

cheat::feature::CarList* cheat::feature::CarList::GetInstance() {
	static CarList role;
	return &role;
}
const cheat::FeatureGUIInfo& cheat::feature::CarList::GetGUIInfo() const {
	static const FeatureGUIInfo info{ "列表", "载具", true };
	return info;
}
void cheat::feature::CarList::DrawMain() {
	ImGui::Text(std::format("{} 个载具", Car::vector.GetValue().size()).c_str());
	if (ImGui::BeginTable("PickitemList", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("地址", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& obj : Car::vector.GetValue()) {
			ImGui::TableNextRow();
			ImGui::PushID(&obj);
			try {
				if (!IsBadReadPtr(obj, sizeof(void*))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text("%s[%s]", name[Car::CarDataId.Get(obj)->ToString()].data(), Car::CarDataId.Get(obj)->ToString().data());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text("%p", static_cast<void*>(obj));
				}
			} catch (const SEH_exception& e) {
				LOG_ERROR("%s", e.what());
			} catch (...) {
				LOG_ERROR("error");
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
bool cheat::feature::CarList::NeedStatusDraw() const { return Feature::NeedStatusDraw(); }
void cheat::feature::CarList::DrawStatus() { Feature::DrawStatus(); }
bool cheat::feature::CarList::NeedInfoDraw() const { return Feature::NeedInfoDraw(); }
void cheat::feature::CarList::DrawInfo() { Feature::DrawInfo(); }
void cheat::feature::CarList::DrawExternal() { Feature::DrawExternal(); }
cheat::feature::CarList::CarList() { }
