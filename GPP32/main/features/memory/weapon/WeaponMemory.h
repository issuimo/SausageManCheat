#pragma once
#include "../../../main.h"
class WeaponMemory : public Feature {
public:
	WeaponMemory() = default;
	~WeaponMemory() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> WeaponMemory*;
private:
	inline static bool allAuto;
};

