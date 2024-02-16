#pragma once
#include "../../main.h"

class ObscuredCheatingDetector {
public:

	static auto AntiStartDetection() -> void {
		I::Get("ACTk.Runtime.dll")->Get("ObscuredCheatingDetector")->Get<I::Method>("StopDetection", {})->Invoke<void>();
	}

private:
};
