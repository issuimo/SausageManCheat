#pragma once
#include "../main.h"
#include "Base/thread-safe.h"

#include "PickItemDataConfig.h"
#include "PickItemNet.h"

class PickItem : II::MonoBehaviour {
public:
	inline static IF::Variable<PickItemDataConfig*, PickItem> pickItemData;
	inline static IF::Variable<PickItemNet*, PickItem> MyPickItemNet;

	static auto Update() -> void {
		if (pClass) {
			static std::vector<PickItem*> temp;
			temp.clear();

			bool noError = true;

			VEH* pVEH = new VEH();
			pVEH->SetCallback([&](const SEH_exception& s, PEXCEPTION_POINTERS p) {
				LOG_ERROR("%s", s.what());
				noError = false;
			});

			if (!noError) {
				delete pVEH;
				return;
			}
			pVEH->CaptureContext();

			for (const auto item : pClass->FindObjectsByType<PickItem*>()) {
				if (!IsBadReadPtr(item, sizeof(void*)) && !IsBadReadPtr(MyPickItemNet.Get(item), sizeof(void*)) && !IsBadReadPtr(pickItemData.Get(item), sizeof(void*)))
					temp.push_back(item);
			}

			delete pVEH;
			vector = temp;
		}
	}

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItem");
		pickItemData.Init(pClass->Get<IF>("pickItemData"));
		MyPickItemNet.Init(pClass->Get<IF>("MyPickItemNet"));

		PickItemNet::Init();
		PickItemDataConfig::Init();
	}

	inline static SafeValue<std::vector<PickItem*>> vector = std::vector<PickItem*>(0);
	inline static I::Class* pClass;
};
