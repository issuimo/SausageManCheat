#pragma once
#include "../../../main.h"
namespace cheat::feature {
	class RoleAimBot : public cheat::Feature {
	public:

		config::Field<bool> f_AimBot;
		config::Field<bool> f_AimBotLock;
		config::Field<Hotkey> f_AimBotHotKey;
		config::Field<float> f_AimBotX;
		config::Field<float> f_AimBotY;
		config::Field<float> f_AimBotSpeedY;
		config::Field<float> f_AimBotSpeedX;
		config::Field<float> f_AimBotPrejudgment;
		config::Field<bool> f_AimBotWeak;

		static RoleAimBot* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
		void Updata();
	protected:
		RoleAimBot();
	};
}
