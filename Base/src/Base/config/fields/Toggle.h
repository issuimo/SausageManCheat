#pragma once

#include <Base/Hotkey.h>
#include <Base/config/internal/FieldBase.h>

namespace config {
	template <typename T>
	class Toggle {
	public:
		bool enabled;
		T value;

		Toggle(const T& value) : enabled(false), value(value) {}

		Toggle(bool enabled) : enabled(enabled), value() {}

		Toggle() : enabled(false), value() {}

		operator bool&() { return enabled; }

		operator T&() { return value; }

		inline auto operator==(Toggle& rhs) -> bool { return rhs.enabled == enabled && rhs.value == value; }
	};

	// Okay, close your eyes and don't look at this mess. (Please)
	template <typename T>
	class Field<Toggle<T>> : public internal::FieldBase<Toggle<T>> {
	public:
		using base = internal::FieldBase<Toggle<T>>;
		using base::operator=;
		using base::base;

		operator bool() const { return base::value(); }

		operator T&() const { return base::value().value; }
	};
}

template <typename T>
struct nlohmann::adl_serializer<config::Toggle<T>> {
	static auto to_json(json& j, const config::Toggle<T>& toggle) -> void { j = {{"toggled", toggle.enabled}, {"value", config::converters::ToJson(toggle.value)}}; }

	static auto from_json(const json& j, config::Toggle<T>& toggle) -> void {
		if (j.is_boolean()) {
			toggle.enabled = j;
			toggle.value = {};
			return;
		}

		toggle.enabled = j["toggled"].get<uint32_t>();
		config::converters::FromJson(toggle.value, j.contains("value") ? j["value"] : j["hotkey"]); // Support previously version
	}
};
