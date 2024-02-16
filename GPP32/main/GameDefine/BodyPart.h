#pragma once
#include "../main.h"

class BodyPart : II::MonoBehaviour {
public:
	BodyPart* _this;
	II::BoxCollider* boxCollider;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Get(BodyPart* player) -> BodyPart* {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("BodyPart", "HitType");
		if (pClass && player) {
			const auto role = new BodyPart();
			if (!IsBadReadPtr(player, 10)) {
				try {
					role->_this = player;
					role->boxCollider = role->F<II::BoxCollider*>("_boxCollider");
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
