#pragma once

#include "../../main.h"


struct NetworkBase : II::MonoBehaviour {

	char gameWorld[0x000004];
	int assetId;
	char mirror[0x000004];
	char netBaseType[0x000004];
};