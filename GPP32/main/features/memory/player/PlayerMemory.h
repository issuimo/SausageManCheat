#pragma once
#include "../../../main.h"
class PlayerMemory : public Feature {
public:
	PlayerMemory() = default;
	~PlayerMemory() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> PlayerMemory*;
private:
	struct {
		bool enable;
		float v = 1;
	}scale;
};

