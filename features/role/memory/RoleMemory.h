#pragma once
#include "../../../main.h"
namespace cheat::feature {
	class RoleMemory : public cheat::Feature {
	public:

		config::Field<bool> f_Transmit;
		config::Field<float> f_TransmitDistance;
		config::Field<Hotkey> f_TransmitHotKey;
		config::Field<float> f_TransmitX;
		config::Field<float> f_TransmitY;

		static RoleMemory* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
		static bool catchError;
		static void CallBack(const SEH_exception& s, PEXCEPTION_POINTERS& p);
		void OnKeyUp(short key, bool& cancelled);
		void Updata();
	protected:
		RoleMemory();
	};
}
