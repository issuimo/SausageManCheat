#pragma once
#include "../../../main.h"
class CameraList : public Feature {
public:
	CameraList() = default;
	~CameraList() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> CameraList*;
private:
};

