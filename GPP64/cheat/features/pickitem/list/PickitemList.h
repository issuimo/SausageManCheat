#pragma once

#include "../../../main.h"
namespace cheat::feature {
	class PickitemList : public cheat::Feature {
	public:

		static PickitemList* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
	protected:
		PickitemList();
	};
}
