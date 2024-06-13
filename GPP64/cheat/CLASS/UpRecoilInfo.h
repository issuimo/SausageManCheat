#pragma once
#include "../main.h"

struct UpRecoilInfo : II::Object {
	int bulletNum;
	float UpMinRecoil;
	char UpMaxRecoil[0x000004];
};