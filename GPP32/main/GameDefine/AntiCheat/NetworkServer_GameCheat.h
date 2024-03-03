#pragma once
#pragma once
#include "../../main.h"
#include "../Role/RoleNet.h"
#include "../Network/IGameWorldMessage.h"

#include "CmdCheckRoleSize.h"

struct NetworkServer_GameCheat : II::Object {

	static void AntiStartDetection() {
		pClass = I::Get("Assembly-CSharp.dll")->Get("NetworkServer_GameCheat");
		onCmdMemoryCheatCallBack = pClass->Get<IM>("OnCmdMemoryCheatCallBack", { "RoleNet", "IGameWorldMessage" })->Cast<void, NetworkServer_GameCheat*, RoleNet*, IGameWorldMessage*>();
		onCmdCheckRoleSizeCallBack = pClass->Get<IM>("OnCmdCheckRoleSizeCallBack", { "RoleNet" , "Proto_GameCheat.CmdCheckRoleSize" })->Cast<void, NetworkServer_GameCheat*, RoleNet*, CmdCheckRoleSize*>();
		onCmdCheckRoleSizeCallBack2 = pClass->Get<IM>("OnCmdCheckRoleSizeCallBack", { "RoleNet" , "IGameWorldMessage" })->Cast<void, NetworkServer_GameCheat*, RoleNet*, IGameWorldMessage*>();
		veh::Hook(onCmdMemoryCheatCallBack, OnCmdMemoryCheatCallBack);
		veh::Hook(onCmdCheckRoleSizeCallBack, OnCmdCheckRoleSizeCallBack);
		veh::Hook(onCmdCheckRoleSizeCallBack2, OnCmdCheckRoleSizeCallBack2);
	}

	static void OnCmdMemoryCheatCallBack(NetworkServer_GameCheat*, RoleNet*, IGameWorldMessage*) {
		LOG_WARNING("NetworkServer_GameCheat->OnCmdMemoryCheatCallBack is call \n");
		return;
	}

	static void OnCmdCheckRoleSizeCallBack(NetworkServer_GameCheat*, RoleNet*, CmdCheckRoleSize*) {
		LOG_WARNING("NetworkServer_GameCheat->OnCmdCheckRoleSizeCallBack (CmdCheckRoleSize) is call \n");
		return;
	}

	static void OnCmdCheckRoleSizeCallBack2(NetworkServer_GameCheat*, RoleNet*, IGameWorldMessage*) {
		LOG_WARNING("NetworkServer_GameCheat->OnCmdCheckRoleSizeCallBack (IGameWorldMessage) is call \n");
		return;
	}

	inline static  IC* pClass;
	inline static IM::MethodPointer<void, NetworkServer_GameCheat*, RoleNet*, IGameWorldMessage*> onCmdMemoryCheatCallBack;
	inline static IM::MethodPointer<void, NetworkServer_GameCheat*, RoleNet*, CmdCheckRoleSize*> onCmdCheckRoleSizeCallBack;
	inline static IM::MethodPointer<void, NetworkServer_GameCheat*, RoleNet*, IGameWorldMessage*> onCmdCheckRoleSizeCallBack2;
};
