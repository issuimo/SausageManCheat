﻿#pragma once
#include "../../../main.h"
class ItemEsp : public Feature {
public:
	ItemEsp() = default;
	~ItemEsp() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> ItemEsp*;
private:
	inline static bool enable;
	struct {
		bool name;
		bool num;
	}setting;
};

