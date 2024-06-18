#pragma once

#include "../main.h"
#include "CarNet.h"
#include "SOCarDataConfig.h"

#include "Base/thread-safe.h"

struct Car : II::MonoBehaviour {
	inline static IF::Variable<II::String*, Car> CarDataId;
	inline static IF::Variable<II::Transform*, Car> myTransform;

	static void CallBack(const SEH_exception& s, PEXCEPTION_POINTERS& p) {
		LOG_ERROR("error");
	}

	static auto Update() -> void {
		if (pClass) {
			_set_se_translator(SEH_exception::TranslateSEHtoCE);
			UnityResolve::ThreadAttach();

			static std::vector<Car*> temp{};
			temp.clear();

			VEH* pVEH = new VEH();
			pVEH->SetCallback(CallBack);
			pVEH->CaptureContext();
			
			for (const auto pRole : pClass->FindObjectsByType<Car*>()) {
				if (!IsBadReadPtr(pRole, sizeof(Car))) {
					temp.push_back(pRole);
				}
			}

			delete pVEH;
			vector = temp;
		}
	}

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("Car");
		CarDataId.Init(pClass->Get<IF>("CarDataId"));
		myTransform.Init(pClass->Get<IF>("myTransform"));
	}

	inline static SafeValue<std::vector<Car*>> vector = std::vector<Car*>(0);
	inline static I::Class* pClass;
};
