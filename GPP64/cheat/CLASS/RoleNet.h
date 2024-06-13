#pragma once

#include "../main.h"
#include "NetworkBase.h"
#include "Role.h"

struct RoleNet : NetworkBase {
	UnityResolve::UnityType::Transform* myTransform;
	UnityResolve::UnityType::GameObject* myGameObj;
	char loops[0x000004];
	char roleNetClient[0x000004];
	char roleNetServer[0x000004];
	char MyRole[0x000004];
	bool IsLookRole;
	char IsLookRole_[0x000003];
	char _lookRoleInfo[0x000004];
	char myNetworkTransform[0x000004];
	char mirror[0x000004];
	char myStateData[0x000004];
};