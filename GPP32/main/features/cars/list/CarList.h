#pragma once
#include "../../../main.h"
class CarList : public Feature {
public:
	CarList() = default;
	~CarList() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> CarList*;
	inline static std::unordered_map<std::string, std::string> name{
		{"HoverBoard", "悬浮滑板"},
		{"ArmoredBus", "装甲巴士"},
		{"UFO", "小飞碟"},
		{"Machine_Carrier", "机甲"},
		{"PonyVehicle", "小马"},
		{"JetCar", "飞车"},
		{"Jeep", "吉普车"},
		{"Buggy", "蹦蹦车"},
		{"TRexKing", "霸王龙"},
		{"Dragon", "呆呆龙"},
		{"Peterosaur", "翼龙"},
		{"Triceratops", "憨憨龙"},
		{"Raptors", "奔奔龙"},
		{"Kayak", "快艇"},
	};

	inline static std::unordered_map<std::string, std::string> nameZh;
};

