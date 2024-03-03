#pragma once
#include "../../main.h"
#include "../Network/NetworkBase.h"

struct CarNet : NetworkBase {

	UnityResolve::UnityType::Transform* myTransform;
	bool isSetName;
	bool IsMystery;
	char IsMystery_[0x000002];
	char carNetClient[0x000004];
	char carNetServer[0x000004];
	char MyStartGame[0x000004];
	char loops[0x000004];
	UnityResolve::UnityType::Vector3 MovePoint;
	UnityResolve::UnityType::Quaternion MoveRota;
	char mDragon[0x000004];
	char mRobot[0x000004];
	char MySoCarData[0x000004];
	char MySoCarSkinData[0x000004];
	bool _IsIgnoreAirBombArea_k__BackingField;
	char _IsIgnoreAirBombArea_k__BackingField_[0x000003];
	char mirror[0x000004];
	char OnInit[0x000004];

};