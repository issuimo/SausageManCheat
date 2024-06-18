#pragma once

#include "../../../main.h"
namespace cheat::feature {
	class CameraList : public cheat::Feature {
	public:

		static CameraList* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;
	protected:
		CameraList();
	};
}
