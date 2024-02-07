#pragma once
#include "../main.h"

class InjectionDetector
{
public:
	InjectionDetector();
	~InjectionDetector();

	static auto AntiStartDetection() -> void {
		I::Get("ACTk.Runtime.dll")->Get("InjectionDetector")->Get<I::Method>("StopDetection", {})->Invoke<void>();
	}

private:
};