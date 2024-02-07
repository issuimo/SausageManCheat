#pragma once
#include "../main.h"

class PickItemNet : public II::Object {
public:
	void* _this;
	std::int64_t id;
	II::Vector3 syncPoint;
	int bulletNum;
	int isClear;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Get(PickItemNet* ptr) -> PickItemNet* {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItemNet");
		if (pClass) {
			if (!IsBadReadPtr(ptr, 10)) {
				const auto ptr2 = new PickItemNet();
				try {
					ptr2->_this = ptr;
					ptr2->id = ptr2->F<std::int64_t>("ItemId");
					ptr2->bulletNum = ptr2->F<int>("BulletNum");
					ptr2->isClear = ptr2->F<bool>("IsClear");
					ptr2->syncPoint = ptr2->F<II::Vector3>("SyncPoint");
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
