#pragma once
#include "../../main.h"

class Analyze : public Feature {
public:
	Analyze() = default;
	~Analyze() = default;

	auto GetInfo() const -> const GuiInfo & override;
	auto Draw() -> void override;
	auto Render() -> void override;
	auto Update() -> void override;
	auto Save(nlohmann::json& json) -> void override;
	auto Load(nlohmann::json& json) -> void override;

	static auto GetInstance() -> Analyze*;

	inline static std::mutex time_mutex;
	inline static std::unordered_map<std::string, float> time;
private:
	bool color;
	float r, g, b;
};

#define TIME_LOCK std::lock_guard analyze_time_lock(Analyze::time_mutex);
#define TIME_START(index) auto beforeTime##index = std::chrono::steady_clock::now();
#define TIME_END(index) auto afterTime##index = std::chrono::steady_clock::now();
#define TIME_UPDATA(x, index) Analyze::time[x] = std::chrono::duration<double, std::micro>(afterTime##index - beforeTime##index).count();