#include "main.h"

#include "GameDefine/AntiCheat/InjectionDetector.h"
#include "GameDefine/AntiCheat/ObscuredCheatingDetector.h"
#include "GameDefine/AntiCheat/RoleCheat.h"
#include "GameDefine/AntiCheat/RoleCheatConfig.h"
#include "GameDefine/Role/RoleNet.h"
#include "GameDefine/AntiCheat/NetworkServer_GameCheat.h"
#include "GameDefine/Camera/CameraMove.hpp"
#include "GameDefine/Pickitem/PickItem.h"

#include "features/camera/list/CameraList.h"
#include "features/items/esp/ItemEsp.h"
#include "features/items/list/ItemList.h"
#include "features/cars/list/CarList.h"
#include "features/cars/esp/CarEsp.h"
#include "features/player/esp/PlayerEsp.h"
#include "features/player/list/PlayerList.h"
#include "features/setting/setting.h"
#include "features/setting/analyze.h"
#include "features/setting/debug.h"
#include "features/memory/player/PlayerMemory.h"
#include "features/memory/weapon/WeaponMemory.h"
#include "features/player/aim/PlayerAim.h"

auto Main::InitFeatures() -> void {
#define Anti(name) \
	LOG_INFO(std::string("关闭->") + #name + "\n"); \
	name::AntiStartDetection()
	Anti(InjectionDetector);
	Anti(ObscuredCheatingDetector);
	Anti(RoleCheat);
	Anti(RoleCheatConfig);
	Anti(NetworkServer_GameCheat);

#define InitFunction(name) name::Init(); \
	LOG_INFO(std::string("初始化->") + #name + "\n");
	InitFunction(Role);
	InitFunction(RoleNet);
	InitFunction(PickItem);
	InitFunction(CameraMove);

#define ADD(name) \
	LOG_INFO(std::string("添加->") + #name + "\n"); \
	Feature::features[name::GetInstance()->GetInfo().tableName].push_back(reinterpret_cast<Feature*>(name::GetInstance()))
	ADD(PlayerList);
	ADD(PlayerEsp);
	ADD(ItemList);
	ADD(ItemEsp);
	ADD(CameraList);
	ADD(Setting);
	ADD(PlayerMemory);
	ADD(PlayerAim);
	ADD(Debug);
	ADD(WeaponMemory);
	ADD(Analyze);
	ADD(CarList);
	ADD(CarEsp);
}
