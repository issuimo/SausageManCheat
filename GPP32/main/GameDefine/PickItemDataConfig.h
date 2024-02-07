#pragma once
#include "../main.h"

class PickItemDataConfig : public II::Object {
public:
	void* _this;
	std::int64_t id;
	std::string itemName;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Get(PickItemDataConfig* ptr) -> PickItemDataConfig* {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItemDataConfig");
		if (pClass) {
			if (!IsBadReadPtr(ptr, 10)) {
				const auto ptr2 = new PickItemDataConfig();
				try {
					ptr2->_this = ptr;
					ptr2->id = ptr2->F<std::int64_t>("ItemId");
					ptr2->itemName = ptr2->F<II::String*>("itemName")->ToString();
				}
				catch (...) {
					ERROR();
					delete ptr2;
					return nullptr;
				}
				return ptr2;
			}
		}
		return nullptr;
	}

	static auto M(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pClass->Get<I::Method>(name, args); }
private:
	inline static I::Class* pClass;
};
