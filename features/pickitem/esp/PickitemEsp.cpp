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
		catch (...) { LOG_ERROR("error"); }
		ImGui::EndTabBar();
	}
}

auto cheat::feature::PickitemEsp::NeedStatusDraw() const -> bool { return f_enable; }
auto cheat::feature::PickitemEsp::DrawStatus() -> void { ImGui::Text("物品透视"); }

float dis = 0;
bool singleGun = false;
auto cheat::feature::PickitemEsp::NeedInfoDraw() const -> bool { return singleGun; }
auto cheat::feature::PickitemEsp::DrawInfo() -> void {
	ImGui::Text("信号枪[%.3f]", dis);
}

auto cheat::feature::PickitemEsp::DrawExternal() -> void {
	static MemoryPool<Item> itemPool;

	if (!f_enable) {
		singleGun = false;
		return;
	}

	const auto bg = ImGui::GetBackgroundDrawList();
	static std::vector<std::vector<Item*>> list;
	static std::vector<Item*> tempList;

	auto clearItemList = [&](std::vector<Item*>& itemList) {
		for (auto& item : itemList) {
			itemPool.deleteElement(item);
		}
		itemList.clear();
	};

	clearItemList(tempList);
	list.clear();

	II::Camera* cam = nullptr;
	try {
		const auto cams = II::Camera::GetAllCamera();
		if (!cams.empty()) cam = cams[0];
	}
	catch (...) {
		LOG_ERROR("error");
		return;
	}

	if (!cam) return;

	singleGun = false;
	dis = 0.0f;

	const auto& pickItems = PickItem::vector.GetValue();
	tempList.reserve(pickItems.size());

	II::Vector3 mPoint;
	if (Role::localRole && !IsBadReadPtr(Role::localRole, sizeof(void*))) {
		try {
			mPoint = RoleLogic::NowPoint.Get(Role::roleLogic.Get(Role::localRole));
		} catch (...) {}
	}

	for (const auto& obj : pickItems) {
		try {
			if (IsBadReadPtr(obj, sizeof(void*)) ||
				IsBadReadPtr(PickItem::pickItemData.Get(obj), sizeof(void*)) ||
				IsBadReadPtr(PickItem::MyPickItemNet.Get(obj), sizeof(void*))) {
				continue;
			}

			const auto nPoint = PickItemNet::SyncPoint.Get(PickItem::MyPickItemNet.Get(obj));
			auto sPoint = cam->WorldToScreenPoint(nPoint);
			sPoint.y = renderer::windowHeight - sPoint.y;
			if (sPoint.z <= 0) continue;

			auto item = itemPool.newElement();
			try {
				item->name = PickItemDataConfig::itemName.Get(PickItemNet::MyPickItemData.Get(PickItem::MyPickItemNet.Get(obj)))->ToString();
			}
			catch (...) {
				LOG_ERROR("error");
				itemPool.deleteElement(item);
				continue;
			}

			if (!m_Sections.contains(item->name)) {
				itemPool.deleteElement(item);
				continue;
			}

			auto& info = m_Sections[item->name];
			if (item->name == "信号枪") {
				singleGun = true;
				float diff[3] = { nPoint.x - mPoint.x, nPoint.y - mPoint.y, nPoint.z - mPoint.z };
				dis = item->distance = static_cast<int>(cblas_snrm2(3, diff, 1));
			}

			if (info.value().show) {
				item->color = info.value().color;
				float diff[3] = { nPoint.x - mPoint.x, nPoint.y - mPoint.y, nPoint.z - mPoint.z };
				item->distance = static_cast<int>(cblas_snrm2(3, diff, 1));
				item->sPoint = { sPoint.x, sPoint.y };
				item->hasGroup = false;
				tempList.push_back(item);
			}
			else {
				itemPool.deleteElement(item);
			}
		}
		catch (...) {
			LOG_ERROR("error");
		}
	}

	for (size_t i = 0; i < tempList.size(); ++i) {
		if (tempList[i]->hasGroup) continue;

		std::vector<Item*> temp;
		temp.push_back(tempList[i]);
		tempList[i]->hasGroup = true;

		for (size_t j = i + 1; j < tempList.size(); ++j) {
			if (tempList[j]->hasGroup) continue;

			float diff[2] = { tempList[i]->sPoint.x - tempList[j]->sPoint.x, tempList[i]->sPoint.y - tempList[j]->sPoint.y };
			if (cblas_snrm2(2, diff, 1) < 200.0f) {
				tempList[j]->hasGroup = true;
				temp.push_back(tempList[j]);
			}
		}

		list.push_back(std::move(temp));
	}

	for (const auto& group : list) {
		if (group.empty()) continue;

		float y = group[0]->sPoint.y - ((group.size() / 2) * 15.0f);
		for (const auto& item : group) {
			ImVec2 sPoint = { group[0]->sPoint.x, y };
			DrawTextWithOutline(bg, sPoint, std::format("{}m | {}", item->distance, item->name).c_str(), item->color, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
			y += 15.0f;
		}
	}

	for (const auto& item : tempList) {
		if (!item->hasGroup) {
			DrawTextWithOutline(bg, item->sPoint, std::format("{}m | {}", item->distance, item->name).c_str(), item->color, 1, OutlineSide::All, ImColor{ 0, 0, 0 });
		}
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
