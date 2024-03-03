#pragma once
#include "../../main.h"
#include "PickItemDataConfig.h"
#include "PickItemNet.h"
#include "../../features/items/esp/ItemEsp.h"
#include "../Role/Role.h"

class PickItem : II::MonoBehaviour {
public:
	PickItemDataConfig* pickItemData;
	UnityResolve::UnityType::GameObject* ItemObj;
	UnityResolve::UnityType::Transform* _myTransform;
	UnityResolve::UnityType::GameObject* myGameObj;
	int _autoItemId;
	bool _isClear;
	bool _isCanPick;
	bool _isAddEffect;
	bool isFlySKy;
	UnityResolve::UnityType::Vector3 _nowPint;
	PickItemNet* MyPickItemNet;
	char _MyPickItemHit_k__BackingField[0x000004];
	bool _IsSetRole_k__BackingField;
	char _IsSetRole_k__BackingField_[0x000003];
	int _MapIndexSign_k__BackingField;
	UnityResolve::UnityType::Vector3 _ChildStartPos_k__BackingField;
	UnityResolve::UnityType::Transform* _ChildParentTransForm_k__BackingField;
	UnityResolve::UnityType::String* _effectName;
	UnityResolve::UnityType::GameObject* _effectObj;
	char gameWorld[0x000004];
	bool isPick;
	char isPick_[0x000003];
	char clock[0x000004];
	UnityResolve::UnityType::GameObject* MySkin;
	UnityResolve::UnityType::String* _skinSign;
	char myCircusBallSkin[0x000004];
	char hitList[0x000004];
	int checkNum;
	bool _isUp;
	char _isUp_[0x000003];
	UnityResolve::UnityType::Vector3 _jumpPoint;
	float _jumpDeplay;
	UnityResolve::UnityType::Vector3 _startPoint;
	UnityResolve::UnityType::Vector3 _endPoint;
	float upSpeed;
	float leftSpeed;
	float forwardSpeed;
	UnityResolve::UnityType::Vector3 addVec3;
	bool isGetAniInfo;
	bool _isStartFlyEvent;
	char _isStartFlyEvent_[0x000002];
	UnityResolve::UnityType::String* FlyEffectName;
	UnityResolve::UnityType::GameObject* flyEffect;
	float _nowSpeed;
	float _addSpeed;
	float _maxSpeed;
	float _minSpeed;
	bool isNeedAddSize;
	bool _isAddLowHpEffect;
	char _isAddLowHpEffect_[0x000002];
	UnityResolve::UnityType::String* _lowHpEffectName;
	UnityResolve::UnityType::GameObject* _lowHpEffectObj;
	char AnimCollider[0x000004];

	static auto Update() -> void {
		if (!pClass) pClass = I::Get("Assembly-CSharp.dll")->Get("PickItem");
		if (pClass) {
			std::vector<PickItem*> temp;
			try {
				[&] {
					__try {
						[&]() {
							for (const auto item : pClass->FindObjectsByType<PickItem*>()) {
								if (!IsBadReadPtr(item, sizeof(PickItem)) && !IsBadReadPtr(item->MyPickItemNet, sizeof(PickItemNet)) && !IsBadReadPtr(item->pickItemData, sizeof(PickItemDataConfig))) {
									temp.push_back(item);
								}
							}
						}();
					} __except (EXCEPTION_EXECUTE_HANDLER) {
						[] {
							ERROR("PickItem->FindObjectsByType (except)");
						}();
					}
				}();
			} catch (const std::exception&) {
				ERROR("PickItem->FindObjectsByType (catch)");
			}
			std::lock_guard lock(mutex);
			vector = temp;
		}
	}

	static void Init() {
		pClass = I::Get("Assembly-CSharp.dll")->Get("PickItem");
		if (pClass) {
			awake = pClass->Get<IM>("Awake")->Cast<void, PickItem*>();
			clear = pClass->Get<IM>("clear")->Cast<void, PickItem*>();
			// veh::Hook(awake, Awake);
			// veh::Hook(clear, Clear);
		}
	}

	static void Awake(PickItem* _this) {
		veh::CallOriginal<void>(awake, _this);
		allVector.push_back(_this);
		return;
	}

	static auto Clear(PickItem* _this) -> void {
		veh::CallOriginal<void>(clear, _this);
		if (const auto it = std::ranges::find(allVector, _this); it != allVector.end()) {
			allVector.erase(it);
		}
		return;
	}

	inline static std::mutex mutex;
	inline static std::vector<PickItem*> vector;
	inline static std::vector<PickItem*> allVector;
	inline static I::Class* pClass;
	inline static IM::MethodPointer<void, PickItem*> awake;
	inline static IM::MethodPointer<void, PickItem*> clear;
};
