#pragma once
#include "../../main.h"

struct RoleControlBase : II::MonoBehaviour {
	float roleSizeCenterY;
	float roleSizeCenterZ;
	float roleSizeHeight;
	float roleSizeRadius;
	UnityResolve::UnityType::Transform* _myTransform;
	UnityResolve::UnityType::GameObject* _myGameObj;
	char MyRole[0x000004];
};