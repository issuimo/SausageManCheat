#pragma once

#include "../../../main.h"
namespace cheat::feature {
	class RoleESp : public cheat::Feature {
	public:

		config::Field<bool> f_Enable;
		config::Field<bool> f_Box;
		config::Field<bool> f_Info;
		config::Field<bool> f_Equip;
		config::Field<bool> f_Line;
		config::Field<bool> f_Bone;
		config::Field<bool> f_Weak;
		config::Field<bool> f_View;
		config::Field<bool> f_Solid;

		static RoleESp* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
	protected:
		RoleESp();
	};
}
