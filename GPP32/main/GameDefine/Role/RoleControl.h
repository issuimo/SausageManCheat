#pragma once
#include "../../main.h"
#include "../AnimatorControl.h"
#include "../BodyPart.h"

class RoleControl : II::MonoBehaviour {
public:
	RoleControl* _this;
	float height;
	float radius;
	AnimatorControl* animatorControl;
	BodyPart* bodyPart;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	template <typename T>
	auto BF(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pBClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Get(RoleControl* player) -> RoleControl* {
		if (!pClass || !pBClass) {
			pClass = I::Get("Assembly-CSharp.dll")->Get("RoleControl");
			pBClass = I::Get("Assembly-CSharp.dll")->Get("RoleControlBase");
		}

		if (pClass && player) {
			const auto role = new RoleControl();
			if (!IsBadReadPtr(player, 10)) {
				try {
					role->_this = player;
					role->height = role->BF<float>("roleSizeHeight");
					role->radius = role->BF<float>("roleSizeRadius");
					role->animatorControl = AnimatorControl::Get(role->F<AnimatorControl*>("animatorControl"));
					role->bodyPart = BodyPart::Get(role->F<BodyPart*>("myHitType"));

					if (!role->animatorControl || !role->bodyPart) throw "";

					return role;
				}
				catch (...) {
					ERROR()
					delete role->animatorControl;
					delete role->bodyPart;
					delete role;
				}
			}
		}
		return nullptr;
	}

	auto Del() const -> void {
		delete animatorControl;
		delete bodyPart;
	}

	static auto M(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pClass->Get<I::Method>(name, args); }
	static auto BM(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pBClass->Get<I::Method>(name, args); }
private:
	inline static I::Class* pClass;
	inline static I::Class* pBClass;
};
