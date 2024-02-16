#pragma once
#include "../../../main.h"
class PlayerEsp : public Feature {
public:
	PlayerEsp() = default;
	~PlayerEsp() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> PlayerEsp*;
private:
	bool enable;

	enum class BoxStyle : int {
		Quad,
		ThreeD
	};

	struct {
		bool box;
		bool info;
		bool line;
		bool bone;
		BoxStyle boxStyle{BoxStyle::Quad};
	}setting;
};

