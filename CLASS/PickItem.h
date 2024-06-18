#pragma once
#include "../main.h"
#include "Base/thread-safe.h"

#include "PickItemDataConfig.h"
#include "PickItemNet.h"

class PickItem : II::MonoBehaviour {
public:
	inline static IF::Variable<PickItemDataConfig*, PickItem> pickItemData;
	inline static IF::Variable<PickItemNet*, PickItem> MyPickItemNet;

	static void CallBack(const SEH_exception& s, PEXCEPTION_POINTERS& p) {
		LOG_ERROR("error");
	}

	static auto Update() -> void {
		if (pClass) {
			_set_se_translator(SEH_exception::TranslateSEHtoCE);
			UnityResolve::ThreadAttach();

			static std::vector<PickItem*> temp;
			temp.clear();

			VEH* pVEH = new VEH();
			pVEH->SetCallback(CallBack);
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
