#include "CarList.h"
#include "../../../GameDefine/Car/Car.h"

auto CarList::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ "载具", "列表", true, false, false }; }
void CarList::Draw() { }
void CarList::Render() {
	std::lock_guard lock(Car::mutex);
	ImGui::Text((std::format("{} 辆载具", Car::vector.size())).c_str());
	if (ImGui::BeginTable("CarList", 2,
		ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
		ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
		ImGui::TableSetupScrollFreeze(1, 1);
		ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("油量", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();

		for (const auto& car : Car::vector) {
			ImGui::TableNextRow();
			ImGui::PushID(&car);
			try {
				if (!IsBadReadPtr(car, sizeof(Car)) && !IsBadReadPtr(car->soCarData, sizeof(SOCarDataConfig))) {
					if (ImGui::TableSetColumnIndex(0)) ImGui::Text(car->CarDataId->ToString().c_str());
					if (ImGui::TableSetColumnIndex(1)) ImGui::Text(std::format("{}", car->soCarData->OilConsumption).c_str());
				}
			} catch (...) {
				ERROR("CarList->nullptr")
			}
			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}
void CarList::Update() { Feature::Update(); }
void CarList::Save(nlohmann::json& json) { Feature::Save(json); }
void CarList::Load(nlohmann::json& json) { Feature::Load(json); }
auto CarList::GetInstance() -> CarList* {
	static CarList cars;
	for (auto [str, zh] : name) {
		nameZh[(str)] = (zh);
	}
	return &cars;
}
