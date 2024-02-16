#pragma once
#include "../../main.h"
#include "RoleLogic.h"
#include "RoleControl.h"

class Role {
public:
	II::MonoBehaviour* _this;
	RoleLogic* roleLogic;
	RoleControl* roleControl;
	II::Vector3 sPoint;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Update() -> void {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("Role");
		try {
			if (pClass) {
				std::vector<Role*> temp;
				Role*              local = nullptr;
				for (const auto pRole : pClass->FindObjectsByType<Role*>()) {
					const auto role = new Role();
					if (!IsBadReadPtr(pRole, 10)) {
						try {
							role->_this = reinterpret_cast<II::MonoBehaviour*>(pRole);
							role->roleLogic = RoleLogic::Get(role->F<RoleLogic*>("roleLogic"));
							role->roleControl = RoleControl::Get(role->F<RoleControl*>("MyRoleControl"));
							role->sPoint = role->F<II::Vector3>("sPoint");

							if (!role->roleLogic || !role->roleControl) throw "";
							if (role->roleLogic->isLocalRole) local = role;
							temp.push_back(role);
						} catch (...) {
							ERROR()
							delete role->roleLogic;
							if (role->roleControl) {
								role->roleControl->Del();
								delete role->roleControl;
							}
							delete role;
						}
					}
				}
				std::lock_guard lock(mutex);
				for (const auto role : vector) {
					delete role->roleLogic;
					role->roleControl->Del();
					delete role->roleControl;
					delete role;
				}
				vector = temp;
				localRole = local;
			}
		}
		catch (...) {
			ERROR()
		}
	}

	static auto M(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pClass->Get<I::Method>(name, args); }

	inline static std::mutex mutex;
	inline static Role* localRole;
	inline static std::vector<Role*> vector;
private:
	inline static I::Class* pClass;
};
