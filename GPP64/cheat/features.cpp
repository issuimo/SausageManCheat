#include "features.h"

#include "main.h"

#include "Base/cheat/misc/Settings.h"

#include "CLASS/PickItem.h"
#include "CLASS/Role.h"

#include "features/misc/About/About.h"
#include "features/misc/Camera/CameraList.h"
#include "features/misc/Debug/Debug.h"
#include "features/misc/Debug/DllDebug.h"
#include "features/misc/Hotkeys/Hotkeys.h"
#include "features/pickitem/esp/PickitemEsp.h"
#include "features/pickitem/list/PickitemList.h"
#include "features/role/esp/RoleEsp.h"
#include "features/role/list/RoleList.h"

auto InitFeatures() -> void {

	Role::Init();
	PickItem::Init();

	const auto manager = Main::Instance();

#define FEAT_INST(name) cheat::feature::name::GetInstance()
	manager->AddFeatures({
		FEAT_INST(Settings),
		FEAT_INST(Hotkeys),
		FEAT_INST(About),
		FEAT_INST(Debug),
		FEAT_INST(DllDebug),
		FEAT_INST(CameraList),
		FEAT_INST(PickitemList),
		FEAT_INST(PickitemEsp),
		FEAT_INST(RoleESp),
		FEAT_INST(RoleList),
	});

	manager->SetModuleOrder({
		"玩家",
		"物品",
		"载具",
		"热键",
		"设置",
		"关于",
		"调试"
	});
}

auto InitEvent() -> void {
	std::thread([] {
		_set_se_translator(SEH_exception::TranslateSEHtoCE);
		UnityResolve::ThreadAttach();
		while (true) {
			Sleep(50);
			Main::GameUpdateEvent(); 
		}
	}).detach();

	std::thread([] {
		tp::ThreadPool pool(2);
		_set_se_translator(SEH_exception::TranslateSEHtoCE);
		UnityResolve::ThreadAttach();
		while (true) {
			Sleep(200);
			pool.push(Role::Update);
			pool.push(PickItem::Update);
			pool.wait_until_done();
		}
	}).detach();
}