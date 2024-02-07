#pragma once
#include "PickItemNet.h"

#include "../main.h"

class RoleLogic : public II::MonoBehaviour {
public:
	void* _this;
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
	auto         F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Update() -> void {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("RoleLogic");
		try {
			if (pClass) {
				std::vector<RoleLogic*> temp;
				for (const auto player : pClass->FindObjectsByType<RoleLogic*>()) {
					const auto role = new RoleLogic();
					if (!IsBadReadPtr(player, 10)) {
						try {
							role->_this    = player;
							role->name     = role->F<II::String*>("NickName")->ToString();
							role->hp       = role->F<float>("hp");
							role->breathValue = role->F<float>("BreathValue");
							role->weakValue = role->F<float>("WeakValue");
							role->power = role->F<float>("Power");
							role->isLocalRole = role->F<bool>("isLocalRole");
							role->teamNum = role->F<std::int64_t>("TeamNum");
							role->teamRoleIndex = role->F<int>("teamRoleIndex");
							role->nowPoint = role->F<II::Vector3>("NowPoint");
							temp.push_back(role);
							if (role->isLocalRole) localPlayer = role;
						} catch (...) {
							ERROR();
							delete role;
						}
					}
				}
				std::lock_guard lock(mutex);
				localPlayer = nullptr;
				for (const auto roleLogic : vector) delete roleLogic;
				vector = temp;
			}
		} catch (...) {
			ERROR();
		}
	}

	static auto M(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pClass->Get<I::Method>(name, args); }

	inline static std::mutex mutex;
	inline static std::vector<RoleLogic*> vector;
	inline static RoleLogic* localPlayer;

	static auto AddPickItem(const PickItemNet* item) -> void {
		static I::Method* method;
		if (!method) method = M("addPickItem", { "*" });
		if (method) {
			std::lock_guard lock(mutex);
			for (const auto element : vector) {
				if (element->isLocalRole) {
					method->Invoke<void>(item->_this);
					LOG_INFO("call addPickItem\n");
				}
			}
		}
	}

	static auto OnPickItem(const PickItemNet* item, const bool isAutoPick = true, const bool isReLogin = false) -> void {
		static I::Method* method;
		if (!method) method = M("OnPickItem", { "*", "*", "*" });
		if (method) {
			std::lock_guard lock(mutex);
			for (const auto element : vector) {
				if (element->isLocalRole) {
					method->Invoke<void>(item->_this, isAutoPick, isReLogin);
					LOG_INFO("call OnPickItem\n");
				}
			}
		}
	}
private:
	inline static I::Class* pClass;
};
