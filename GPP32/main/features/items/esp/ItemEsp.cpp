#include "ItemEsp.h"
#include "../../../GameDefine/PickItem/PickItem.h"
#include "../../../GameDefine/Role/Role.h"

#include "../../setting/analyze.h"

auto ItemEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ (const char*)u8"物品", (const char*)u8"透视", true, false, true }; }
void ItemEsp::Draw() {
	if (enable) {
		std::lock_guard lock(PickItem::mutex);
		const auto bg = ImGui::GetBackgroundDrawList();
		try {
			II::Vector3 mPoint{}; {
				std::lock_guard lock2(Role::mutex);
				if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(RoleLogic))) {
					mPoint = Role::localRole->roleLogic->NowPoint;
				}
			}

			TIME_LOCK;
			TIME_START(1);
			std::vector<std::vector<Item>> list;
			std::vector<Item> tempList;
			tempList.reserve(PickItem::vector.size());
			list.reserve(5);
			for (const auto& obj : PickItem::vector) {
				if (!IsBadReadPtr(obj, sizeof(PickItem)) && !IsBadReadPtr(obj->pickItemData, sizeof(PickItemDataConfig)) && !IsBadReadPtr(obj->MyPickItemNet, sizeof(PickItemNet))) {
					auto sPoint = WorldToScreenPoint(obj->MyPickItemNet->SyncPoint);
					if (sPoint.z < 0) { continue; }
					auto nPoint = obj->MyPickItemNet->SyncPoint;
					Item item;
					item.name = obj->pickItemData->itemName;
					ItemInfo info;
					info.show = false;
					for (auto& [name, pinfo] : itemInfoList) {
						if (item.name->ToString().find(name) != std::string::npos) {
							info = pinfo;
							break;
						}
					}
					if (info.show) {
						item.color = info.color;
						item.distance = (int)sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2));
						item.sPoint = DrawHelp::BoxScreen::ToVec2(sPoint);
						item.hasGroup = false;
						tempList.push_back(item);
					}
				}
			}
			TIME_END(1);
			TIME_UPDATA("ItemEsp::PickItem::vector", 1);

			TIME_START(2);
			for (size_t i = 0; i < tempList.size(); i++) {
				if (tempList[i].hasGroup) {
					continue;
				}
				std::vector<Item> temp;
				temp.reserve(tempList.size() - i);
				for (size_t j = i + 1; j < tempList.size(); j++) {
					if (tempList[j].hasGroup) {
						continue;
					}
					if (sqrt(pow(tempList[i].sPoint.x - tempList[j].sPoint.x, 2) + pow(tempList[i].sPoint.y - tempList[j].sPoint.y, 2)) < 150.0f) {
						tempList[j].hasGroup = true;
						temp.push_back(tempList[j]);
					}
				}
				list.push_back(temp);
			}
			TIME_END(2);
			TIME_UPDATA("ItemEsp::PickItem::group", 2);

			TIME_START(3);
			for (auto group : list) {
				if (!group.empty()) {
					float y = group[0].sPoint.y + -((group.size() / 2) * 15.0f);
					for (auto item : group) {
						item.sPoint.y = y;
						item.sPoint.x = group[0].sPoint.x;
						DrawTextWithOutline(bg, item.sPoint, std::format("{}m | {}", item.distance, item.name->ToString()).c_str(), item.color, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
						y += 15.0f;
					}
				}
			}
			

			for (auto item : tempList) {
				if (!item.hasGroup) {
					DrawTextWithOutline(bg, item.sPoint, std::format("{}m | {}", item.distance, item.name->ToString()).c_str(), item.color, 1, DrawHelp::OutlineSide::All, ImColor{ 0,0,0 });
				}
			}
			TIME_END(3);
			TIME_UPDATA("ItemEsp::PickItem::draw", 3);
		} catch (...) {
			ERROR("ItemEsp->nullptr")
		}
	}
}
void ItemEsp::Render() {
	ImGui::Checkbox((const char*)u8"启用", &enable);
	if (enable && ImGui::BeginTabBar("List")) {
		if (ImGui::BeginTabItem(GbkToUtf8("子弹").c_str())) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == GbkToUtf8("子弹") && info.type.empty()) {
						ImGui::PushID(name.c_str());
						ImGui::TableNextRow();
						if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
						if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

						ImGui::PopID();
					}
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(GbkToUtf8("配件").c_str())) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == GbkToUtf8("配件") && info.type.empty()) {
						ImGui::PushID(name.c_str());
						ImGui::TableNextRow();
						if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
						if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

						ImGui::PopID();
					}
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(GbkToUtf8("装备").c_str())) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == GbkToUtf8("装备") && info.type.empty()) {
						ImGui::PushID(name.c_str());
						ImGui::TableNextRow();
						if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
						if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

						ImGui::PopID();
					}
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(GbkToUtf8("投掷物").c_str())) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == GbkToUtf8("投掷物") && info.type.empty()) {
						ImGui::PushID(name.c_str());
						ImGui::TableNextRow();
						if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
						if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

						ImGui::PopID();
					}
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(GbkToUtf8("药品").c_str())) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == GbkToUtf8("药品") && info.type.empty()) {
						ImGui::PushID(name.c_str());
						ImGui::TableNextRow();
						if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
						if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

						ImGui::PopID();
					}
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(GbkToUtf8("其他").c_str())) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == GbkToUtf8("其他") && info.type.empty()) {
						ImGui::PushID(name.c_str());
						ImGui::TableNextRow();
						if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
						if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

						ImGui::PopID();
					}
				}

				ImGui::EndTable();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(GbkToUtf8("武器").c_str())) {
			for (auto& typeName : type) {
				if (ImGui::CollapsingHeader(GbkToUtf8(typeName).c_str())) {
					if (ImGui::BeginTable("List", 2,
						ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
						ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
						ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
						ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
						ImGui::TableSetupScrollFreeze(1, 1);
						ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
						ImGui::TableHeadersRow();

						for (auto& [name, info] : itemInfoList) {
							if (info.group == GbkToUtf8("武器") && GbkToUtf8(typeName) == info.type) {
								ImGui::PushID(name.c_str());
								ImGui::TableNextRow();
								if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
								if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

								ImGui::PopID();
							}
						}

						ImGui::EndTable();
					}
				}
			}
			if (ImGui::CollapsingHeader(GbkToUtf8("近战").c_str())) {
				if (ImGui::BeginTable("List", 2,
					ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
					ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
					ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
					ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn((const char*)u8"显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn((const char*)u8"名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : itemInfoList) {
						if (info.group == GbkToUtf8("近战") && info.type.empty()) {
							ImGui::PushID(name.c_str());
							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("", &info.show);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}
void ItemEsp::Update() { Feature::Update(); }
void ItemEsp::Save(nlohmann::json& json) { 
	json["ItemESP"]["enable"] = enable;
	int index{ 0 };
	for (auto& [name, info] : itemInfoList) {
		json["ItemESP"]["itemlist"][std::to_string(index)]["show"] = info.show;
		index++;
	}
}
void ItemEsp::Load(nlohmann::json& json) { 
	if (json.contains("ItemESP")) {
		if (json["ItemESP"].contains("enable")) {
			enable = json["ItemESP"]["enable"];
		}
		int index{ 0 };
		if (json["ItemESP"].contains("itemlist")) {
			for (auto& [name, info] : itemInfoList) {
				if (json["ItemESP"]["itemlist"].contains(std::to_string(index))) {
					info.show = json["ItemESP"]["itemlist"][std::to_string(index)]["show"];
				}
				index++;
			}
		}
	}
}
auto ItemEsp::GetInstance() -> ItemEsp* {
	static ItemEsp ItemEsp;
	for (auto& [name, info] : infoList) {
		ItemInfo item;
		item.color = info.color;
		item.group = GbkToUtf8(info.group);
		item.show = info.show;
		item.type = GbkToUtf8(info.type);
		itemInfoList[GbkToUtf8(name)] = item;
	}
	return &ItemEsp;
}
