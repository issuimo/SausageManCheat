#pragma once
#include "../../main.h"

struct CameraMove : II::MonoBehaviour {

	char myCameraData[0x000004];
	UnityResolve::UnityType::Transform* CameraPointAnim;
	UnityResolve::UnityType::Transform* WarCameraPoint;
	UnityResolve::UnityType::Transform* WarCameraRota;
	char MyCamera[0x000004];
	UnityResolve::UnityType::Transform* MyCameraTranSelf;
	UnityResolve::UnityType::Transform* MyCameraTran;
	UnityResolve::UnityType::Transform* MyCameraRecoilWeapon;
	UnityResolve::UnityType::Transform* LookTarget;
	UnityResolve::UnityType::Transform* LookDirX;
	UnityResolve::UnityType::Transform* LookDirY;
	UnityResolve::UnityType::Transform* FireBox;
	UnityResolve::UnityType::Transform* FireEffectBox;
	UnityResolve::UnityType::Transform* _myTransform;
	UnityResolve::UnityType::GameObject* _myGameObj;
	UnityResolve::UnityType::Quaternion MyCameraRotationX;
	UnityResolve::UnityType::Quaternion MyCameraRotationY;
	UnityResolve::UnityType::Vector3 NowPoint;
	float RotSpeed;
	char LockRole[0x000004];
	UnityResolve::UnityType::Vector3 EndPoint;
	UnityResolve::UnityType::Transform* FireTarget;
	char MyCameraRange[0x000004];
	float cameraClampMinX;
	float cameraClampMaxX;
	float cameraRatio;
	UnityResolve::UnityType::Transform* MyCameraFar;
	char MyCheckPickItem[0x000004];
	char WeaponCamera[0x000004];
	UnityResolve::UnityType::Transform* WeaponCameraTan;
	UnityResolve::UnityType::GameObject* WeaponCameraObj;
	UnityResolve::UnityType::Transform* ShotJitterBox;
	UnityResolve::UnityType::Transform* ShotJitterBoxRota;
	UnityResolve::UnityType::Transform* CameraBoxRota;
	UnityResolve::UnityType::Transform* CheckCameraBox;
	UnityResolve::UnityType::Transform* SideWeapoonBox;
	UnityResolve::UnityType::Transform* FireEffectRotaBox;
	char SideWeaponAnim[0x000004];
	UnityResolve::UnityType::Transform* BombLookBox;
	UnityResolve::UnityType::Transform* BombLookRota;
	UnityResolve::UnityType::Vector3 BombLookBoxLocalPos;
	char hookAssistantBox[0x000004];
	char CollisionCheckPoint[0x000004];
	UnityResolve::UnityType::Transform* LimitHitBox;
	int _isOpenGyroscope;
	bool _isRoleCameraRotate;
	bool _IsControlRoleRotate_k__BackingField;
	bool IsBirthIslandMove;
	char IsBirthIslandMove_[0x000001];
	char gameWorld[0x000004];
	bool isDirDowned;
	char isDirDowned_[0x000003];
	char holdXCCRole[0x000004];
	bool IsLock;
	bool IsSetLockRole;
	char IsSetLockRole_[0x000002];
	float shakeCDTime;
	UnityResolve::UnityType::Transform* _lockTarget;
	UnityResolve::UnityType::String* nowFirstShootState;
	float lastLevel;
	bool _isNomoCameraFar;
	bool _isBirthIslandNomoCameraFar;
	char _isBirthIslandNomoCameraFar_[0x000002];
	UnityResolve::UnityType::Vector3 _moveEndPoint;
	int cullingMask;
	float _cannonSencitive;
	float robotTransformDelayMoveTime;
	int lastScreenWidth;
	bool _IsBombLook;
	bool _syncLookTarget;
	char _syncLookTarget_[0x000002];
	float _fallYSpeed;
	float normalSpeed;
	bool isDown;
	bool isLerpCamera;
	bool isAllDead;
	bool isPlayDeadCamera;
	bool isPlayCSModeDeadCamera;
	char isPlayCSModeDeadCamera_[0x000003];
	UnityResolve::UnityType::Vector3 attackRolePos;
	float _parabolaJumpDeplay;
	UnityResolve::UnityType::Vector3 _heightPoint;
	bool _isParabolaMove;
	char _isParabolaMove_[0x000003];
	UnityResolve::UnityType::Vector3 _parabolaEndPoint;
	UnityResolve::UnityType::Vector3 _parabolaStartPoint;
	float _parabolaNowSpeed;
	float deadCameraSpeed;
	float carDistance;
	float carHeight;
	float carHeightDamping;
	float carRotationDamping;
	float currentCarRotationDamping;
	float recoverDelayTime;
	float recoverCameraDelayTime;
	bool isRestCamera;
	bool isLockCarMove;
	char isLockCarMove_[0x000002];
	UnityResolve::UnityType::Vector3 DragLocalEulerAngles;
	UnityResolve::UnityType::Vector3 CameraJumpFallowTarget;
	bool isStartBallisticTest;
	bool isNeedResetCarCamera;
	char isNeedResetCarCamera_[0x000002];
	float _deployBZTime;
	UnityResolve::UnityType::Quaternion _cameraRotationX;
	UnityResolve::UnityType::Quaternion _cameraRotationY;
	UnityResolve::UnityType::Quaternion _lookRotationX;
	UnityResolve::UnityType::Quaternion _lookRotationY;
	bool currentIsOnCar;
	char currentIsOnCar_[0x000003];
	int currentCarState;
	UnityResolve::UnityType::Vector3 farDistanceOffset;
	float curveTime;
	float curveAllTime;
	char animationCurvePosData[0x000010];
	char animationCurveRotateData[0x000010];
	char animationCurvePosAllData[0x000010];
	char animationCurveRotateAllData[0x000010];
	UnityResolve::UnityType::Vector3 lastAnimCurvePosValue;
	UnityResolve::UnityType::Transform* buffCameraRatio;
	float _nowFieldOfView;
	float _newCameraRatio;
	float cameraRatioLevel;
	float _deployCameraRole;
	UnityResolve::UnityType::Vector3 _checkCameraPoint;
	char checkCameraRoleHitList[0x000004];
	bool isHitWater;
	char isHitWater_[0x000003];
	float _xRot;
	float _yRot;
	float _diffJLSPX;
	bool _isFlyLook;
	char _isFlyLook_[0x000003];
	float _openJLSRotaX;
	float _openJLSRotaY;
	char dirMoveCountList[0x000004];
	char removeDirMoveCountList[0x000004];
	float gauntletCamSpeed;
	float cameraLerpSpeed;
	bool isTestLock;
	char isTestLock_[0x000003];
	UnityResolve::UnityType::Quaternion tigaHopeCameraRotaX;
	UnityResolve::UnityType::Quaternion tigaHopeCameraRotaY;
	bool isCanTurnCamera;
	bool _isReBackLookDir;
	char _isReBackLookDir_[0x000002];
	UnityResolve::UnityType::Quaternion _endLookXDirPoint;
	UnityResolve::UnityType::Quaternion _endLookYDirPoint;
	bool _isStartMyCameraRotationY;
	char _isStartMyCameraRotationY_[0x000003];
	UnityResolve::UnityType::Quaternion startMyCameraRotationY;
	float _startMyCameraRotationY;
	bool _isStartRevert;
	char _isStartRevert_[0x000003];
	float _fireDeploy;
	float _recoilY;
	float _fireRevertEventTime;
	float _userFireRevertTime;
	bool _isSetWeaponBox;
	char _isSetWeaponBox_[0x000003];
	UnityResolve::UnityType::Vector3 _lockZXPoint;
	float _lockTime;
	char CameraShakePoint[0x000004];
	char CameraShakeRota[0x000004];
	UnityResolve::UnityType::Transform* CameraShake;
	char myTweenPosition[0x000004];
	char myTweenRotation[0x000004];
	UnityResolve::UnityType::Transform* LookZXTarget;
	char hitList[0x000004];
	UnityResolve::UnityType::Vector3 StandPointA;
	UnityResolve::UnityType::Vector3 StandPointB;
	float StandShotSpeed;
	float StandHeight;
	UnityResolve::UnityType::Vector3 RunPointA;
	UnityResolve::UnityType::Vector3 RunPointB;
	float RunShotSpeed;
	float RunHeight;
	UnityResolve::UnityType::Vector3 _jumpPoint;
	float _jumpDeplay;
	UnityResolve::UnityType::Vector3 _startPoint;
	UnityResolve::UnityType::Vector3 _endPoint;
	float _index;
	float _shotJitterSpeed;
	bool _isRun;
	bool _isHuXi;
	bool _isRunToStand;
	char _isRunToStand_[0x000001];
	float _nowSpeed;
	float _defaultSensitive;
	int _sideState;
	int _lastRoleSide;
	float StartDistance;
	float StartOffestY;
	float MoveAnimTime;
	float FlyMoveAnimTime;
	char MoveEase[0x000004];
	float MoveDistance;
	float RationAnimTime;
	UnityResolve::UnityType::Vector3 m_StartPoint;
	bool _isCameraMoveToLookRole;
	char _isCameraMoveToLookRole_[0x000003];
	UnityResolve::UnityType::Vector3 _ScreenCenterPoint;
	bool IsCanFire;
	char IsCanFire_[0x000003];
	float FireCheckDistance;
	float waitCheckTime;
	bool IsCanAidMei;
	char IsCanAidMei_[0x000003];
	char earthQuakeRotation[0x000004];
	char earthQuakePosition[0x000004];
	char weaponQuakeRotation[0x000004];
	char weaponQuakePosition[0x000004];
	char earthQuakeRotationBase[0x000004];
	char earthQuakePositionBase[0x000004];
	char weaponQuakeRotationBase[0x000004];
	char weaponQuakePositionBase[0x000004];
	float earthQuakeTimes;
	char earthQuakeConfig[0x000004];
	bool isStartPlayEarthQuake;
	char isStartPlayEarthQuake_[0x000003];
	UnityResolve::UnityType::Transform* lookAtCopyTrans;
	float lookAtTime;
	bool lookingAt;
	char lookingAt_[0x000003];
	char lookAtComplete[0x000004];

	static void Init() {
		pClass = I::Get("Assembly-CSharp.dll")->Get("CameraMove");
		if (pClass) {
			awake = pClass->Get<IM>("Init")->Cast<void, CameraMove*>();
			clear = pClass->Get<IM>("Clear")->Cast<void, CameraMove*>();
			setLookDir = pClass->Get<IM>("setLookDir")->Cast<void, CameraMove*, float, float>();
			setPointLocalEulerAngles = pClass->Get<IM>("SetPointLocalEulerAngles")->Cast<void, CameraMove*, II::Vector3>();
			setLocalEulerAngles = pClass->Get<IM>("SetLocalEulerAngles")->Cast<void, CameraMove*, II::Vector3>();
			setTranRota = pClass->Get<IM>("SetTranRota")->Cast<void, CameraMove*, II::Quaternion>();
			setLookTarget = pClass->Get<IM>("setLookTarget")->Cast<void, CameraMove*, II::Transform*>();
			veh::Hook(awake, Awake);
			veh::Hook(clear, Clear);
		}
	}

	static auto Awake(CameraMove* _this) -> void {
		veh::CallOriginal<void>(awake, _this);
		allVector.push_back(_this);
		return;
	}

	static auto Clear(CameraMove* _this) -> void {
		veh::CallOriginal<void>(clear, _this);
		if (const auto it = std::ranges::find(allVector, _this); it != allVector.end()) {
			allVector.erase(it);
		}
		return;
	}

	auto SetLookDir(const float xangle, const float yangle) -> void {
		setLookDir(this, xangle, yangle);
	}

	auto SetPointLocalEulerAngles(const II::Vector3& angles) -> void {
		setPointLocalEulerAngles(this, angles);
	}

	auto SetLocalEulerAngles(const II::Vector3& angles) -> void {
		setPointLocalEulerAngles(this, angles);
	}

	auto SetTranRota(const II::Quaternion& rota) -> void {
		setTranRota(this, rota);
	}

	auto SetLookTarget(II::Transform* tran) -> void {
		setLookTarget(this, tran);
	}

	inline static std::mutex mutex;
	inline static std::vector<CameraMove*> vector;
	inline static std::vector<CameraMove*> allVector;
	inline static I::Class* pClass;
	inline static IM::MethodPointer<void, CameraMove*> awake;
	inline static IM::MethodPointer<void, CameraMove*> clear;
	inline static IM::MethodPointer<void, CameraMove*, float, float> setLookDir;
	inline static IM::MethodPointer<void, CameraMove*, II::Vector3> setPointLocalEulerAngles;
	inline static IM::MethodPointer<void, CameraMove*, II::Vector3> setLocalEulerAngles;
	inline static IM::MethodPointer<void, CameraMove*, II::Quaternion> setTranRota;
	inline static IM::MethodPointer<void, CameraMove*, II::Transform*> setLookTarget;
};