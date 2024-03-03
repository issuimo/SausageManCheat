#pragma once
#include "../../../main.h"
#include "../../../GameDefine/Role/Role.h"

class PlayerAim : public Feature {
public:
	PlayerAim() = default;
	~PlayerAim() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> PlayerAim*;
private:
	bool enable;
	bool teleport;
	float xSpeed = 20;
	float ySpeed = 20;
	int aimKey = 16;
	std::string keyName = "Shift";
	bool hasObj = false;
	ImVec2 screenPoint{ 0,0 };
	float rangeX = 100;
	float rangeY = 100;
	Role* aimPlayer;
	bool readyAim;
	float readyValue = 1;
	bool aimLock;
	bool aimWeak;
	float dis;
};

