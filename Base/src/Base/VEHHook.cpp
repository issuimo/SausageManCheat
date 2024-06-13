#include "VEHHook.h"

#include <ranges>

auto veh::Setup() -> void {
	GetSystemInfo(&system_info);
	handle = AddVectoredExceptionHandler(1, VectoredExceptionHandler);
}

auto veh::Hook(void* source, void* destination) -> bool {
	if (!handle) return false;

	MEMORY_BASIC_INFORMATION source_info;
	if (!VirtualQuery(source, &source_info, sizeof(MEMORY_BASIC_INFORMATION))) return false;

	hooks[source] = destination;
	DWORD tmp;
	VirtualProtect(source, system_info.dwPageSize, PAGE_EXECUTE_READ | PAGE_GUARD, &tmp);
	return true;
}

auto veh::Destroy() -> void {
	for (const auto& source : hooks | std::views::keys) {
		DWORD tmp;
		VirtualProtect(source, system_info.dwPageSize, PAGE_EXECUTE_READ, &tmp);
	}

	RemoveVectoredExceptionHandler(handle);
	hooks.clear();
	handle = nullptr;
}

auto CALLBACK veh::VectoredExceptionHandler(EXCEPTION_POINTERS* exception_info) -> LONG {
	if (exception_info->ExceptionRecord->ExceptionCode == EXCEPTION_GUARD_PAGE) {
		if (hooks.contains(exception_info->ExceptionRecord->ExceptionAddress)) {
#ifdef _WIN64
			exception_info->ContextRecord->Rip = (DWORD64)hooks[exception_info->ExceptionRecord->ExceptionAddress];
#else
			exception_info->ContextRecord->Eip = (DWORD)hooks[exception_info->ExceptionRecord->ExceptionAddress];
#endif
		}

		exception_info->ContextRecord->EFlags |= PAGE_GUARD;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	if (exception_info->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
		for (const auto& source : hooks | std::views::keys) {
			DWORD tmp;
			VirtualProtect(source, system_info.dwPageSize, PAGE_EXECUTE_READ | PAGE_GUARD, &tmp);
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
