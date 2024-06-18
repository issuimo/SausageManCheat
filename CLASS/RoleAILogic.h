#pragma once
#include "thread-safe.h"

#include "../main.h"
struct RoleAILogic {
	inline static IF::Variable<II::Vector3, RoleAILogic> Position;
	inline static IF::Variable<int, RoleAILogic> Hp;
	inline static IF::Variable<II::String*, RoleAILogic> Name;

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("RoleAILogic");
		Position.Init(pClass->Get<IF>("<Position>k__BackingField"));
		Hp.Init(pClass->Get<IF>("<Hp>k__BackingField"));
		Name.Init(pClass->Get<IF>("<Name>k__BackingField"));
	}

	static void CallBack(const SEH_exception& s, PEXCEPTION_POINTERS& p) {
		LOG_ERROR("error");
	}

	static auto Update() -> void {
		if (pClass) {
			_set_se_translator(SEH_exception::TranslateSEHtoCE);
			UnityResolve::ThreadAttach();

			static std::vector<RoleAILogic*> temp{};
			temp.clear();

			VEH* pVEH = new VEH();
			pVEH->SetCallback(CallBack);
			pVEH->CaptureContext();

			for (const auto pRole : pClass->FindObjectsByType<RoleAILogic*>()) {
				if (!IsBadReadPtr(pRole, sizeof(void*))) {
					temp.push_back(pRole);
				}
			}

			delete pVEH;
			vector = temp;
		}
	}

	inline static I::Class* pClass;
	inline static SafeValue<std::vector<RoleAILogic*>> vector = std::vector<RoleAILogic*>({});
};
