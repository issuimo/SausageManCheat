#pragma once

#include "../../../main.h"
namespace cheat::feature {
	class CarList : public cheat::Feature {
	public:

		static CarList* GetInstance();
		const cheat::FeatureGUIInfo& GetGUIInfo() const override;

		void DrawMain() override;
		bool NeedStatusDraw() const override;
		void DrawStatus() override;
		bool NeedInfoDraw() const override;
		void DrawInfo() override;
		void DrawExternal() override;

		inline static phmap::flat_hash_map<std::string, std::string> name{
		{"HoverBoard", "悬浮滑板"},
		{"ArmoredBus", "装甲巴士"},
		{"UFO", "小飞碟"},
		{"Machine_Carrier", "机甲"},
		{"PonyVehicle", "小马"},
		{"JetCar", "飞车"},
		{"Jeep", "吉普车"},
		{"Buggy", "蹦蹦车"},
		{"TRexKing", "霸王龙"},
		{"Dragon", "呆呆龙"},
		{"Peterosaur", "翼龙"},
		{"Triceratops", "憨憨龙"},
		{"Raptors", "奔奔龙"},
		{"Kayak", "快艇"},
		{"FlyingBroom", "飞行扫帚"},
		{"SwordTiger", "剑齿虎"},
		};
	protected:
		CarList();
	};
}
