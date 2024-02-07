#include "main.h"

#include "GameDefine/InjectionDetector.h"
#include "GameDefine/ObscuredCheatingDetector.h"

#include "GameDefine/RoleLogic.h"

#include "features/camera/list/CameraList.h"
#include "features/items/esp/ItemEsp.h"
#include "features/items/list/ItemList.h"
#include "features/player/esp/PlayerEsp.h"
#include "features/player/list/PlayerList.h"
#include "features/setting/setting.h"

auto Main::InitFeatures() -> void {
#define Anti(name) name::AntiStartDetection()
	Anti(InjectionDetector);
	Anti(ObscuredCheatingDetector);

#define Init(name) name::Init()


#define ADD(name) Feature::features[name::GetInstance()->GetInfo().tableName].push_back(reinterpret_cast<Feature*>(name::GetInstance()))
	ADD(PlayerList);
	ADD(PlayerEsp);
	ADD(ItemList);
	ADD(ItemEsp);
	ADD(CameraList);
	ADD(Setting);
}
