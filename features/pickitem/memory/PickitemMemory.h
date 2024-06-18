#pragma once
#include "../../../main.h"
namespace cheat::feature {
	class PickitemMemory : public cheat::Feature {
	public:

		config::Field<bool> f_AutoFrie;

		static PickitemMemory* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
		void Update();

	protected:
		PickitemMemory();
	};
}
