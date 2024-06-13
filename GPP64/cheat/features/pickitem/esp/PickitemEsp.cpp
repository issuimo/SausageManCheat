#include "PickitemEsp.h"

#include "../../../CLASS/PickItem.h"
#include "../../../CLASS/Role.h"

auto cheat::feature::PickitemEsp::GetInstance() -> PickitemEsp* {
	static PickitemEsp role;
	return &role;
}

auto cheat::feature::PickitemEsp::GetGUIInfo() const -> const FeatureGUIInfo& {
	static const FeatureGUIInfo info{"透视", "物品", true};
	return info;
}

auto cheat::feature::PickitemEsp::DrawMain() -> void {
	ConfigWidget(f_enable);
	if (f_enable && ImGui::BeginTabBar("List")) {
		try {
			if (ImGui::BeginTabItem("子弹")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "子弹" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.data());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}
					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("配件")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "配件" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("装备")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "装备" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("投掷物")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "投掷物" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("药品")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "药品" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("身份卡")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "身份卡" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("其他")) {
				if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
					ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
					ImGui::TableHeadersRow();

					for (auto& [name, info] : m_Sections) {
						if (info.value().group == "其他" && info.value().type.empty()) {
							bool checked = info.value().show;
							ImGui::PushID(name.data());

							ImGui::TableNextRow();
							if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
							if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

							if (checked != info.value().show) {
								info.value().show = checked;
								info.FireChanged();
							}

							ImGui::PopID();
						}
					}

					ImGui::EndTable();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("武器")) {
				for (auto& typeName : type) {
					if (ImGui::CollapsingHeader(typeName.c_str())) {
						if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
							ImGui::TableSetupScrollFreeze(1, 1);
							ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
							ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
							ImGui::TableHeadersRow();

							for (auto& [name, info] : m_Sections) {
								if (info.value().group == "武器" && typeName == info.value().type) {
									bool checked = info.value().show;
									ImGui::PushID(name.data());

									ImGui::TableNextRow();
									if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
									if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

									if (checked != info.value().show) {
										info.value().show = checked;
										info.FireChanged();
									}

									ImGui::PopID();
								}
							}

							ImGui::EndTable();
						}
					}
				}

				if (ImGui::CollapsingHeader("近战")) {
					if (ImGui::BeginTable("List", 2, ImGuiTableFlags_ScrollX | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable, ImVec2(0.0F, ImGui::GetTextLineHeightWithSpacing() * 12))) {
						ImGui::TableSetupScrollFreeze(1, 1);
						ImGui::TableSetupColumn("显示", ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn("名称", ImGuiTableColumnFlags_None);
						ImGui::TableHeadersRow();

						for (auto& [name, info] : m_Sections) {
							if (info.value().group == "近战" && info.value().type.empty()) {
								bool checked = info.value().show;
								ImGui::PushID(name.data());

								ImGui::TableNextRow();
								if (ImGui::TableSetColumnIndex(0)) ImGui::Checkbox("启用", &checked);
								if (ImGui::TableSetColumnIndex(1)) ImGui::Text(name.c_str());

								if (checked != info.value().show) {
									info.value().show = checked;
									info.FireChanged();
								}

								ImGui::PopID();
							}
						}

						ImGui::EndTable();
					}
				}
				ImGui::EndTabItem();
			}
		}
		catch (const SEH_exception& error) { LOG_ERROR("%s", error.what()); SEH::ShowTraceStack(nullptr); }
		catch (...) { LOG_ERROR("error"); }
		ImGui::EndTabBar();
	}
}

auto cheat::feature::PickitemEsp::NeedStatusDraw() const -> bool { return f_enable; }
auto cheat::feature::PickitemEsp::DrawStatus() -> void { if (f_enable) ImGui::Text("物品透视"); }
auto cheat::feature::PickitemEsp::NeedInfoDraw() const -> bool { return Feature::NeedInfoDraw(); }
auto cheat::feature::PickitemEsp::DrawInfo() -> void { Feature::DrawInfo(); }

auto cheat::feature::PickitemEsp::DrawExternal() -> void {
	if (f_enable) {
		const auto bg = ImGui::GetBackgroundDrawList();

		static std::vector<std::vector<Item>> list;
		static std::vector<Item> tempList;
		tempList.clear();
		list.clear();

		II::Camera* cam;

		try { if (const auto cams = II::Camera::GetAllCamera(); !cams.empty()) cam = cams[0]; } catch (SEH_exception& e) {
			LOG_ERROR("%s", e.what());
			return;
		} catch (...) {
			LOG_ERROR("error");
			return;
		}

		for (const auto& obj : PickItem::vector.GetValue()) {
			try {
				II::Vector3 mPoint;
				if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(void*))) mPoint = RoleLogic::NowPoint.Get(Role::roleLogic.Get(Role::localRole));
				if (!IsBadReadPtr(obj, sizeof(void*)) && !IsBadReadPtr(PickItem::pickItemData.Get(obj), sizeof(void*)) && !IsBadReadPtr(PickItem::MyPickItemNet.Get(obj), sizeof(void*))) {
					auto sPoint = cam->WorldToScreenPoint(PickItemNet::SyncPoint.Get(PickItem::MyPickItemNet.Get(obj)));
					sPoint.y = renderer::windowHeight - sPoint.y;
					if (sPoint.z <= 0) continue;
					const auto nPoint = PickItemNet::SyncPoint.Get(PickItem::MyPickItemNet.Get(obj));
					Item item;
					item.name = PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(PickItem::MyPickItemNet.Get(obj)))->ToString();
					esp::ESPItem info;
					info.show = false;
					info = m_Sections[item.name];
					if (info.show) {
						item.color = info.color;
						item.distance = static_cast<int>(sqrt(pow(nPoint.x - mPoint.x, 2) + pow(nPoint.y - mPoint.y, 2) + pow(nPoint.z - mPoint.z, 2)));
						item.sPoint = {sPoint.x, sPoint.y};
						item.hasGroup = false;
						tempList.push_back(item);
					}
				}
			}
			catch (const SEH_exception& error) { LOG_ERROR("%s", error.what()); SEH::ShowTraceStack(nullptr); }
			catch (...) { LOG_ERROR("error"); }
		}

		for (size_t i = 0; i < tempList.size(); i++) {
			if (tempList[i].hasGroup) continue;
			static std::vector<Item> temp;
			temp.clear();
			temp.push_back(tempList[i]);
			tempList[i].hasGroup = true;
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
					DrawTextWithOutline(bg, sPoint, std::format("{}m | {}", distance, name).c_str(), color, 1, OutlineSide::All, ImColor{0, 0, 0});
					y += 15.0f;
				}
			}
		}


		for (auto [name, sPoint, distance, hasGroup, color] : tempList) if (!hasGroup) DrawTextWithOutline(bg, sPoint, std::format("{}m | {}", distance, name).c_str(), color, 1, OutlineSide::All, ImColor{0, 0, 0});
	}
}

cheat::feature::PickitemEsp::PickitemEsp() :NF(f_enable, "启用", "Pickitem::Esp", false) {
	int i = 0;
	for (auto& [name, info] : infoList) {
		auto& filters = m_Sections[name];
		LOG_TRACE("%s", name.data());
		const esp::ESPItem newItem(name, info.color, info.type, info.group);
		filters = config::CreateField<esp::ESPItem>(name, name, fmt::format("PickitemESP::Filters::{}", i), false, newItem);
		i++;
	}

	for (auto& v : type) v = v;
}
