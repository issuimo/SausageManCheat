#pragma once
#include <windows.h>
#include "SEH.h"
#include "phmap.h"

class VEH {
public:
	static auto Init() -> bool {
		catchs.reserve(50);
		return AddVectoredExceptionHandler(0, VectoredExceptionHandler);
	}

	VEH() {
		if (const DWORD threadId = GetCurrentThreadId(); !catchs.contains(threadId)) catchs[threadId] = this;
	}

	~VEH() {
		catchs.erase(threadId);
	}

	auto RestoreContext() -> void {
		const auto hModule = GetModuleHandleA("ntdll.dll");
		reinterpret_cast<void(*)(PCONTEXT, _EXCEPTION_RECORD*)>(GetProcAddress(hModule, "RtlRestoreContext"))(&context, nullptr);
	}

	auto CaptureContext() -> CONTEXT& {
		RtlCaptureContext(&context);
		return context;
	}

	auto SetContext(const CONTEXT& context) -> void {
		this->context = context;
	}

	auto GetContext() -> CONTEXT& {
		return context;
	}

	auto SetCallback(const std::function<void(const SEH_exception&, PEXCEPTION_POINTERS)>& callback) -> void {
		if (callback) this->callback = callback;
	}

	static auto TryException() -> void {
		*static_cast<int*>(nullptr) = 0;
	}

private:
	DWORD threadId;
	CONTEXT context;
	std::function<void(const SEH_exception&, PEXCEPTION_POINTERS&)> callback;

	inline static phmap::parallel_flat_hash_map<
		DWORD, VEH*, phmap::priv::hash_default_hash<int>,
		phmap::priv::hash_default_eq<int>,
		phmap::priv::Allocator<std::pair<DWORD, VEH*>>, 4, std::mutex> catchs;

	static auto WINAPI  VectoredExceptionHandler(PEXCEPTION_POINTERS exception) -> LONG {
		if (const DWORD threadId = GetCurrentThreadId(); catchs.contains(threadId)) {
			const VEH* pVEH = catchs[threadId];
			pVEH->callback(SEH_exception{ exception->ExceptionRecord->ExceptionCode, exception }, exception);
			*exception->ContextRecord = pVEH->context;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
};
