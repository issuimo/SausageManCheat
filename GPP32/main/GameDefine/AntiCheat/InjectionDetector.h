#pragma once
#include "../../main.h"

class InjectionDetector {
public:
	InjectionDetector();
	~InjectionDetector();

	static auto AntiStartDetection() -> void {
		pClass = I::Get("ACTk.Runtime.dll")->Get("InjectionDetector");

		addToSceneOrGetExisting = pClass->Get<IM>("AddToSceneOrGetExisting")->Cast<InjectionDetector*>();
		startDetection = pClass->Get<IM>("StartDetection")->Cast<void>();
		startDetection2 = pClass->Get<IM>("StartDetection", { "*" })->Cast<void, void*>();
		startDetectionAutomatically = pClass->Get<IM>("StartDetectionAutomatically")->Cast<void, InjectionDetector*>();
		getComponentName = pClass->Get<IM>("GetComponentName")->Cast<II::String*, InjectionDetector*>();

		veh::Hook(addToSceneOrGetExisting, AddToSceneOrGetExisting);
		veh::Hook(startDetection, StartDetection);
		veh::Hook(startDetection2, StartDetection2);
		veh::Hook(startDetectionAutomatically, StartDetectionAutomatically);
		veh::Hook(getComponentName, GetComponentName);

		pClass->Get<IM>("StopDetection")->Invoke<void>();
		pClass->Get<IM>("Dispose")->Invoke<void>();
	}

	static InjectionDetector* AddToSceneOrGetExisting() {
		auto ret = veh::CallOriginal<InjectionDetector*>(addToSceneOrGetExisting);
		LOG_WARNING("InjectionDetector->AddToSceneOrGetExisting is call \n");
		return ret;
	}

	static void StartDetection() {
		LOG_WARNING("InjectionDetector->StartDetection is call \n");
		return;
	}

	static void StartDetection2(void* ptr) {
		LOG_WARNING("InjectionDetector->StartDetection2 is call \n");
		return;
	}

	static void StartDetectionAutomatically(InjectionDetector* ptr) {
		LOG_WARNING("InjectionDetector->StartDetectionAutomatically is call \n");
		return;
	}

	static II::String* GetComponentName(InjectionDetector* ptr) {
		auto ret = veh::CallOriginal<II::String*>(getComponentName, ptr);
		LOG_WARNING(std::format("InjectionDetector->GetComponentName is call return:{}\n", ret->ToString()));
		return ret;
	}

	inline static IC* pClass;
	inline static IM::MethodPointer<InjectionDetector*> addToSceneOrGetExisting;
	inline static IM::MethodPointer<void> startDetection;
	inline static IM::MethodPointer<void, void*> startDetection2;
	inline static IM::MethodPointer<void, InjectionDetector*> startDetectionAutomatically;
	inline static IM::MethodPointer<II::String*, InjectionDetector*> getComponentName;
};