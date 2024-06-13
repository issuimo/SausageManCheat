#pragma once
#include "../main.h"

#include "Base/thread-safe.h"

#include "RoleControl.h"
#include "RoleLogic.h"

class Role : public II::MonoBehaviour {
public:
	inline static IF::Variable<RoleLogic*, Role> roleLogic;
	inline static IF::Variable<RoleControl*, Role> MyRoleControl;

	static auto Update() -> void {
		if (pClass) {
			_set_se_translator(SEH_exception::TranslateSEHtoCE);
			UnityResolve::ThreadAttach();

			static std::vector<Role*> temp{};
			static Role* local{};
			temp.clear();
			local = nullptr;

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

			for (const auto pRole : pClass->FindObjectsByType<Role*>()) {
				if (!IsBadReadPtr(pRole, sizeof(void*))) {
					if (!IsBadReadPtr(roleLogic.Get(pRole), sizeof(void*)) && RoleLogic::isLocalRole.Get(roleLogic.Get(pRole))) {
						local = pRole;
					}
					temp.push_back(pRole);
				}
			}

			delete pVEH;
			vector = temp;
			localRole = local;
		}
	}

	static void Init() {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("Role");
		roleLogic.Init(pClass->Get<IF>("roleLogic"));
		MyRoleControl.Init(pClass->Get<IF>("MyRoleControl"));

		RoleControl::Init();
		RoleLogic::Init();
	}

	inline static Role* localRole;
	inline static SafeValue<std::vector<Role*>> vector = std::vector<Role*>({});
	inline static I::Class* pClass;
};
