#pragma once
#include "../../main.h"

class RoleLogic {
public:
	RoleLogic* _this;
	std::string name;
	float hp;
	float breathValue;
	float weakValue;
	float power;
	II::Vector3 nowPoint;
	bool isLocalRole;
	std::int64_t teamNum;
	int teamRoleIndex;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Get(RoleLogic* player) -> RoleLogic* {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("RoleLogic");
		if (pClass && player) {
			const auto role = new RoleLogic();
			if (!IsBadReadPtr(player, 10)) {
				try {
					role->_this = player;
					role->name = role->F<II::String*>("NickName")->ToString();
					role->hp = role->F<float>("hp");
					role->breathValue = role->F<float>("BreathValue");
					role->weakValue = role->F<float>("WeakValue");
					role->power = role->F<float>("Power");
					role->isLocalRole = role->F<bool>("isLocalRole");
					role->teamNum = role->F<std::int64_t>("TeamNum");
					role->teamRoleIndex = role->F<int>("teamRoleIndex");
					role->nowPoint = role->F<II::Vector3>("NowPoint");
					return role;
				}
				catch (...) {
					ERROR()
					delete role;
				}
			}
		}
		return nullptr;
	}

	static auto M(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pClass->Get<I::Method>(name, args); }
private:
	inline static I::Class* pClass;
};
