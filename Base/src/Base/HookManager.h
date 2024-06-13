#pragma once

#include <Base/Logger.h>
#include <map>
#include <detours.h>
#include <phmap.h>

#define CALL_ORIGIN(function, ...) \
	HookManager::call(function, __func__, __VA_ARGS__)

class HookManager
{
public:
	template <typename Fn>
	static long install(Fn func, Fn handler) {
		const long ret = enable(func, handler);
		LOG_TRACE("安装处理程序: %p -> %p 结果: %d", func, handler, ret);
		holderMap[reinterpret_cast<void*>(handler)] = reinterpret_cast<void*>(func);
		return ret;
	}

	template <typename Fn>
	static Fn getOrigin(Fn handler, const char* callerName = nullptr) noexcept {
		if (!holderMap.contains(reinterpret_cast<void*>(handler))) {
			LOG_WARNING("没有找到处理程序: %s 的来源。也许是bug.", callerName == nullptr ? "<未知>" : callerName);
			return nullptr;
		}
		return reinterpret_cast<Fn>(holderMap[reinterpret_cast<void*>(handler)]);
	}

	template <typename Fn>
	static long detach(Fn handler) noexcept  {
		const long ret = disable(handler);
		LOG_TRACE("卸载处理程序: %p", handler);
		holderMap.erase(reinterpret_cast<void*>(handler));
		return ret;
	}

	// I don't know why
#ifdef _WIN64

	template <typename RType, typename... Params>
	static RType call(RType(*handler)(Params...), const char* callerName = nullptr, Params... params)
	{
		auto origin = getOrigin(handler, callerName);
		if (origin != nullptr)
			return origin(params...);

		return RType();
	}

#else

	template <typename RType, typename... Params>
	static RType call(RType(__cdecl *handler)(Params...), const char* callerName = nullptr, Params... params) {
		auto origin = getOrigin(handler, callerName);
		if (origin != nullptr)
			return origin(params...);

		return RType();
	}

	template <typename RType, typename... Params>
	static RType call(RType(__stdcall *handler)(Params...), const char* callerName = nullptr, Params... params) {
		auto origin = getOrigin(handler, callerName);
		if (origin != nullptr)
			return origin(params...);

		return RType();
	}

#endif

	static void detachAll() noexcept {
		for (const auto& key : holderMap | std::views::keys) {
			disable(key);
		}
		holderMap.clear();
	}

private:
	inline static phmap::parallel_flat_hash_map< // 这个哈希表是线程安全的
		void*, void*, phmap::priv::hash_default_hash<void*>,
		phmap::priv::hash_default_eq<void*>,
		phmap::priv::Allocator<std::pair<void*, void*>>, 4, std::mutex> holderMap;


	template <typename Fn>
	static long disable(Fn handler) {
		Fn origin = getOrigin(handler);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		long ret = DetourDetach(&reinterpret_cast<PVOID&>(origin), handler);
		DetourTransactionCommit();

		return ret;
	}

	template <typename Fn>
	static long enable(Fn& func, Fn handler) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		long ret = DetourAttach(&reinterpret_cast<PVOID&>(func), handler);
		DetourTransactionCommit();
		return ret;
	}
};


