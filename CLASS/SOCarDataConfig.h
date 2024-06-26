﻿#pragma once
#include "../main.h"

struct SOCarDataConfig : II::Object {
	UnityResolve::UnityType::String* id;
	UnityResolve::UnityType::String* CarSign;
	float Hp;
	float OilConsumption;
	float FuelConsumption;
	int CarRoleNum;
	float CarNoHurtLimit;
	float CarSpeedHurt;
	bool IsInWaterCar;
	char IsInWaterCar_[0x000003];
	int CarState;
	char LostHpEffectValue[0x000004];
	float LostOilEffectValue;
	float SpoiledEffectTime;
	float carDefaultLockTarget;
	float carAngleRito;
	UnityResolve::UnityType::Vector3 CarFirstPoint;
	float showAudioSpeed;
	float bombArea;
	float bombHitValue;
	int flyCarType;
	float flyInTime;
	float flyDownTime;
	float fireCD;
	float noDriverDownSpeed;
	bool isNeedOil;
	bool isCanFire;
	char isCanFire_[0x000002];
	float fireOil;
	char modeMaxHp[0x000004];
	char flyTimes[0x000004];
	char bulletSpeeds[0x000004];
	char bulletGravity[0x000004];
	bool isTransformCar;
	char isTransformCar_[0x000003];
	float dashTime;
	float transformTime;
	float transformCD;
	float dashCD;
	int bulletDamage;
	float delayBombTime;
	float WaitAutoCreateTime;
	float WaitAutoClearTime;
	bool isNeedBomb;
	bool isTouchDimianBomb;
	bool isHaveAiBehaviour;
	char isHaveAiBehaviour_[0x000001];
	float fallDownSpeed;
	float idleOilConsumption;
	float maxShiftTime;
	float btnShiftCD;
	bool passengerCanNotFire;
	bool passengerCanNotThrow;
	bool noBulletHurt;
	char noBulletHurt_[0x000001];
	float maxJumpTime;
	float btnJumpCD;
	UnityResolve::UnityType::String* skill1;
	UnityResolve::UnityType::String* skill2;
	bool rotate;
	bool _Inited_k__BackingField;
	char _Inited_k__BackingField_[0x000003];
	char rawDatas[0x000004];
};