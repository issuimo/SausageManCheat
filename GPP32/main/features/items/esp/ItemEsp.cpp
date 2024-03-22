#include "ItemEsp.h"
#include "../../../GameDefine/PickItem/PickItem.h"
#include "../../../GameDefine/Role/Role.h"

#include "../../setting/analyze.h"

auto ItemEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ ("物品"), ("透视"), true, false, true }; }
void ItemEsp::Draw() {
	if (enable) {
		std::lock_guard lock(PickItem::mutex);
		std::lock_guard lock2(Role::mutex);
		const auto bg = ImGui::GetBackgroundDrawList();
        [&]() {
            __try {
                [&]() {
                    II::Vector3 mPoint{};
                    {
                        if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(RoleLogic))) {
                            mPoint = Role::localRole->roleLogic->NowPoint;
                        }
                    }

                    std::vector<std::vector<Item>> list;
                    std::vector<Item>              tempList;
                    tempList.reserve(PickItem::vector.size());
                    list.reserve(5);
                    for (const auto &obj : PickItem::vector) {
                        if (!IsBadReadPtr(obj, sizeof(PickItem)) && !IsBadReadPtr(obj->pickItemData, sizeof(PickItemDataConfig)) && !IsBadReadPtr(obj->MyPickItemNet, sizeof(PickItemNet))) {
                            auto sPoint = WorldToScreenPoint(obj->MyPickItemNet->SyncPoint);
                            if (sPoint.z < 0) continue;
                            auto nPoint = obj->MyPickItemNet->SyncPoint;
                            Item item;
                            item.name = obj->pickItemData->itemName;
                            ItemInfo info;
                            info.show = false;
                            info      = itemInfoList[item.name->ToString()];
                            if (info.show) {
                                item.color    = info.color;
                                item.distance = static_cast<int>(sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2)));
                                item.sPoint   = DrawHelp::BoxScreen::ToVec2(sPoint);
                                item.hasGroup = false;
                                tempList.push_back(item);
                            }
                        }
                    }

                    for (size_t i = 0; i < tempList.size(); i++) {
                        if (tempList[i].hasGroup) {
                            continue;
                        }
                        std::vector<Item> temp;
                        temp.reserve(tempList.size() - i);
                        for (size_t j = i + 1; j < tempList.size(); j++) {
                            if (tempList[j].hasGroup) continue;
                            if (sqrt(pow(tempList[i].sPoint.x - tempList[j].sPoint.x, 2) + pow(tempList[i].sPoint.y - tempList[j].sPoint.y, 2)) < 150.0f) {
                                tempList[j].hasGroup = true;
                                temp.push_back(tempList[j]);
                            }
                        }
                        list.push_back(temp);
                    }

                    for (auto group : list) {
                        if (!group.empty()) {
                            float y = group[0].sPoint.y + -((group.size() / 2) * 15.0f);
                            for (auto [name, sPoint, distance, hasGroup, color] : group) {
                                sPoint.y = y;
                                sPoint.x = group[0].sPoint.x;
                                DrawTextWithOutline(bg, sPoint, std::format("{}m | {}", distance, name->ToString()).c_str(), color, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                                y += 15.0f;
                            }
                        }
                    }


                    for (auto [name, sPoint, distance, hasGroup, color] : tempList) {
                        if (!hasGroup) {
                            DrawTextWithOutline(bg, sPoint, std::format("{}m | {}", distance, name->ToString()).c_str(), color, 1, DrawHelp::OutlineSide::All, ImColor{0, 0, 0});
                        }
                    }

                }();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                []() {
                    ERROR("ItemEsp (except)");
                }();
            }
        }();
	}
}
void ItemEsp::Render() {
	ImGui::Checkbox(("启用"), &enable);
	if (enable && ImGui::BeginTabBar("List")) {
		if (ImGui::BeginTabItem("子弹")) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn(("显示"), ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn(("名称"), ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("子弹") && info.type.empty()) {
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

		if (ImGui::BeginTabItem("配件")) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("配件") && info.type.empty()) {
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

		if (ImGui::BeginTabItem("装备")) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("装备") && info.type.empty()) {
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

		if (ImGui::BeginTabItem(("投掷物"))) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("投掷物") && info.type.empty()) {
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

		if (ImGui::BeginTabItem(("药品"))) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("药品") && info.type.empty()) {
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

		if (ImGui::BeginTabItem(("身份卡"))) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("身份卡") && info.type.empty()) {
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

		if (ImGui::BeginTabItem(("其他"))) {
			if (ImGui::BeginTable("List", 2,
				ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
				ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (auto& [name, info] : itemInfoList) {
					if (info.group == ("其他") && info.type.empty()) {
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

		if (ImGui::BeginTabItem(("武器"))) {
			for (auto& typeName : type) {
				if (ImGui::CollapsingHeader((typeName).c_str())) {
					if (ImGui::BeginTable("List", 2,
						ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
						ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
						ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
						ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
						ImGui::TableSetupScrollFreeze(1, 1);
						ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
						ImGui::TableHeadersRow();

						for (auto& [name, info] : itemInfoList) {
							if (info.group == ("武器") && (typeName) == info.type) {
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
			if (ImGui::CollapsingHeader(("近战"))) {
				if (ImGui::BeginTable("List", 2,
					ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
					ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
					ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable,
					ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 8))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : itemInfoList) {
						if (info.group == ("近战") && info.type.empty()) {
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
		item.group = (info.group);
		item.show = info.show;
		item.type = (info.type);
		itemInfoList[(name)] = item;
	}
	return &ItemEsp;
}
