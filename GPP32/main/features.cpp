﻿#include "main.h"

#include "GameDefine/AntiCheat/InjectionDetector.h"
#include "GameDefine/AntiCheat/ObscuredCheatingDetector.h"

#include "features/camera/list/CameraList.h"
#include "features/items/esp/ItemEsp.h"
#include "features/items/list/ItemList.h"
#include "features/player/esp/PlayerEsp.h"
#include "features/player/list/PlayerList.h"
#include "features/setting/setting.h"

auto Main::InitFeatures() -> void {
#define Anti(name) \
	LOG_INFO(std::string("关闭->") + #name); \
	name::AntiStartDetection()
	Anti(InjectionDetector);
	Anti(ObscuredCheatingDetector);

#define Init(name) name::Init()


#define ADD(name) \
	LOG_INFO(std::string("添加->") + #name); \
	Feature::features[name::GetInstance()->GetInfo().tableName].push_back(reinterpret_cast<Feature*>(name::GetInstance()))
	ADD(PlayerList);
	ADD(PlayerEsp);
	ADD(ItemList);
	ADD(ItemEsp);
	ADD(CameraList);
	ADD(Setting);
}
