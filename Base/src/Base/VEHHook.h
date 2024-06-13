#pragma once
#include <Windows.h>
#include <phmap.h>
#include <vector>

namespace veh {
	auto          Setup() -> void;
	auto          Hook(void* source, void* destination) -> bool;
	auto          Destroy() -> void;
	auto CALLBACK VectoredExceptionHandler(EXCEPTION_POINTERS* exception_info) -> LONG;

	template <typename ReturnType, typename Prototype, typename... Args> auto CallOriginal(Prototype source, Args... args) -> ReturnType;

	inline SYSTEM_INFO system_info;
	inline PVOID handle;
	inline phmap::parallel_flat_hash_map<void*, void*, phmap::priv::hash_default_hash<void*>, phmap::priv::hash_default_eq<void*>, phmap::priv::Allocator<std::pair<void*, void*>>, 4, std::mutex> hooks;
}

template <typename ReturnType, typename Prototype, typename... Args> auto veh::CallOriginal(Prototype source, Args... args) -> ReturnType {
	DWORD old_protection;

	if constexpr (std::is_void_v<ReturnType>) {
		VirtualProtect(source, system_info.dwPageSize, PAGE_EXECUTE_READ, &old_protection);
		source(args...);
		VirtualProtect(source, system_info.dwPageSize, old_protection, &old_protection);
	} else {
		VirtualProtect(source, system_info.dwPageSize, PAGE_EXECUTE_READ, &old_protection);
		ReturnType result = source(args...);
		VirtualProtect(source, system_info.dwPageSize, old_protection, &old_protection);
		return result;
	}
}
