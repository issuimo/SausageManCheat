#pragma once

#include "../../../main.h"
namespace cheat::feature {
	class CarEsp : public cheat::Feature {
	public:

		config::Field<bool> f_Enable;

		static CarEsp* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
	protected:
		CarEsp();
	};
}
