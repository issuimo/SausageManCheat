#pragma once
#include "../main.h"
#include "PickItemDataConfig.h"
#include "PickItemNet.h"

class PickItem : public II::MonoBehaviour {
public:
	void* _this;
	int checkNum;
	II::Vector3 nowPoint;
	bool isPick;
	PickItemDataConfig* config;
	PickItemNet* net;

	template <typename T>
	auto F(const std::string& field) -> T {
		if (auto ptr = reinterpret_cast<T*>(reinterpret_cast<std::uint32_t>(_this) + reinterpret_cast<std::int32_t>(pClass->Get<std::int32_t>(field))); !IsBadReadPtr(ptr, sizeof(T))) return *ptr;
		return T();
	}

	static auto Update() -> void {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItem");
		try {
			if (pClass) {
				std::vector<PickItem*> temp;
				for (const auto item : pClass->FindObjectsByType<PickItem*>()) {
					const auto pickItem = new PickItem();
					if (!IsBadReadPtr(item, 10)) {
						try {
							pickItem->_this = item;
							pickItem->checkNum = pickItem->F<int>("checkNum");
							pickItem->nowPoint = pickItem->F<II::Vector3>("_nowPint");
							pickItem->isPick = pickItem->F<bool>("isPick");
							pickItem->config = PickItemDataConfig::Get(pickItem->F<PickItemDataConfig*>("pickItemData"));
							pickItem->net = PickItemNet::Get(pickItem->F<PickItemNet*>("MyPickItemNet"));

							if (!pickItem->net) throw "";
							if (!pickItem->config) throw "";
							temp.push_back(pickItem);
						} catch (const char* str) {
							delete pickItem->config;
							delete pickItem->net;
							delete pickItem;
						} catch (...) {
							ERROR();
							delete pickItem->config;
							delete pickItem->net;
							delete pickItem;
						}
					}
				}
				std::lock_guard lock(mutex);
				for (const auto pickItem : vector) {
					delete pickItem->config;
					delete pickItem->net;
					delete pickItem;
				};
				vector = temp;
			}
		}
		catch (...) {
			ERROR();
		}
	}

	static auto M(const std::string& name, const std::vector<std::string>& args = {}) -> I::Method* { return pClass->Get<I::Method>(name, args); }

	inline static std::mutex mutex;
	inline static std::vector<PickItem*> vector;
private:
	inline static I::Class* pClass;
};
