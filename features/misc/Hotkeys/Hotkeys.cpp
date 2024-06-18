#include "Hotkeys.h"
#include "pch.h"

#include "Base/config/Field.h"
#include "Base/config/config.h"
#include "Base/config/fields/Toggle.h"
#include "Base/render/gui-util.h"

namespace cheat::feature {
	Hotkeys::Hotkeys() : Feature() {}

	auto Hotkeys::GetGUIInfo() const -> const FeatureGUIInfo& {
		static const FeatureGUIInfo info{"", "热键", false};
		return info;
	}

	auto Hotkeys::DrawMain() -> void {
		static std::string searchBuffer;
		ImGui::InputText(reinterpret_cast<const char*>(u8"搜索"), &searchBuffer);

		ImGui::BeginChild("Hotkeys");

		std::unordered_map<std::string, std::vector<config::Field<config::Toggle<Hotkey>>*>> sections;

		for (auto& field : config::GetFields<config::Toggle<Hotkey>>()) {
			if (!searchBuffer.empty()) {
				auto name = field.friendName();
				auto it = std::ranges::search(
					name, 
					encode::Utf8ToGbk(searchBuffer),
					[](const char ch1, const char ch2) {
							return std::tolower(ch1) == std::tolower(ch2);
					}).begin();
				if (it == name.end()) {
					continue;
				}
			}

			sections[field.section()].push_back(&field);
		}

		std::vector<config::Field<config::Toggle<Hotkey>>*> singleLineSections;
		std::vector<std::vector<config::Field<config::Toggle<Hotkey>>*>*> multiLineSections;
		for (auto& fields : sections | std::views::values) {
			if (fields.size() == 1) {
				singleLineSections.push_back(fields[0]);
			} else {
				multiLineSections.push_back(&fields);
			}
		}

		for (const auto& field : singleLineSections) {
			ConfigWidget(*field, nullptr, true);
		}

		for (const auto& fields : multiLineSections) {
			if (ImGui::BeginGroupPanel((*fields)[0]->section().c_str(), true)) {
				for (const auto& field : *fields) {
					ConfigWidget(*field, nullptr, true);
				}
			}
			ImGui::EndGroupPanel();
		}

		ImGui::EndChild();
	}

	auto Hotkeys::GetInstance() -> Hotkeys* {
		static Hotkeys instance;
		return &instance;
	}
}
