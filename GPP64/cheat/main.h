#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <pch.h>
#include <Base/httplib.hpp>
#define IMGUI_ENABLE_TEST_ENGINE
#define IMGUI_DEFINE_MATH_OPERATORS
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <winternl.h>

#include "json.hpp"

#include "Base/UnityResolve.hpp"
#include "Base/cheat/CheatManagerBase.h"
#include "Base/Logger.h"
#include "Base/render/gui-util.h"
#include "Base/SEH.h"
#include "Base/VEH.h"
#include "Base/VEHHook.h"
#include "Base/MinHook.h"

namespace internal {
	struct AccountData {
	public:
		uint32_t userID;
		std::string nickName;

		AccountData() : userID(0) {}
	};

	struct AccountConfig {
	public:

		std::unordered_map<uint32_t, std::string>                     pseudos;
		std::unordered_map<uint32_t, AccountData>                     accounts;
		std::unordered_map<uint32_t, std::string>                     id2Profiles;
		std::unordered_map<std::string, std::unordered_set<uint32_t>> profiles2id;

		inline auto operator==(const AccountConfig& other) const -> bool {
			return accounts.size() == other.accounts.size() && pseudos.size() == other.pseudos.size();
		}

		AccountConfig() { }
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AccountData, nickName, userID)

	inline auto to_json(nlohmann::json& nlohmann_json_j, const AccountConfig& nlohmann_json_t) -> void {
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, accounts, pseudos, id2Profiles))
	}

	inline auto from_json(const nlohmann::json& nlohmann_json_j, AccountConfig& nlohmann_json_t) -> void {
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, accounts, pseudos, id2Profiles))

		for (auto& [userID, profileName] : nlohmann_json_t.id2Profiles) {
			auto& profileIDs = nlohmann_json_t.profiles2id[profileName];
			profileIDs.insert(userID);
		}
	}

}

class Main : public cheat::CheatManagerBase {
public:

	static auto Run(HMODULE) -> void;
	static auto Instance() -> Main*;

	auto CursorSetVisibility(bool visibility) -> void final {}
	auto CursorGetVisibility() -> bool final { return true; }

	inline static int port;
	inline static HMODULE myModule{};
	inline static HMODULE gameModule{};
	inline static TEvent<> GameUpdateEvent{};
	inline static TEvent<uint32_t> AccountChangedEvent{};

protected:
	Main();

	enum class ShowType {
		Pseudo,
		UserID,
		Nickname
	};

	config::Field<config::Enum<ShowType>> f_ShowType;

	internal::AccountData m_CurrentAccount;
	config::Field<internal::AccountConfig> f_AccConfig;

	auto IsAccountAttached(uint32_t userID, const std::string& profileName) const -> bool;
	auto GetAccountDisplayName(uint32_t uid) -> std::string;

	auto DetachAccount(uint32_t uid, const std::string& profileName) const -> void;
	auto AttachAccount(uint32_t uid, const std::string& profileName) const -> void;

	auto DrawProfileEntryActivities(const std::string& profileName) -> void final;
	auto DrawProfileEntry(const std::string& profileName) -> void final;
	auto DrawProfileTableHeader() -> void final;
	auto GetProfileTableColumnCount() -> int final;
	auto DrawAccountsList(const std::string& profileName) -> void;
	auto DrawProfileConfiguration() -> void final;
	auto DrawProfileLine() -> void final;
	auto DrawPseudoRename(uint32_t userID) const -> void;

	auto OnAccountChanged(uint32_t userID) -> void;
};

using I = UnityResolve;
using IM = UnityResolve::Method;
using IC = UnityResolve::Class;
using IT = UnityResolve::Type;
using IF = UnityResolve::Field;
using IA = UnityResolve::Assembly;
using II = UnityResolve::UnityType;

typedef struct _SYSTEM_HANDLE {
	ULONG       ProcessId;
	BYTE        ObjectTypeNumber;
	BYTE        Flags;
	USHORT      Handle;
	PVOID       Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG         HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef enum _POOL_TYPE {
	NonPagedPool,
	PagedPool,
	NonPagedPoolMustSucceed,
	DontUseThisType,
	NonPagedPoolCacheAligned,
	PagedPoolCacheAligned,
	NonPagedPoolCacheAlignedMustS
} POOL_TYPE, * PPOOL_TYPE;

typedef struct _OBJECT_TYPE_INFORMATION {
	UNICODE_STRING  Name;
	ULONG           TotalNumberOfObjects;
	ULONG           TotalNumberOfHandles;
	ULONG           TotalPagedPoolUsage;
	ULONG           TotalNonPagedPoolUsage;
	ULONG           TotalNamePoolUsage;
	ULONG           TotalHandleTableUsage;
	ULONG           HighWaterNumberOfObjects;
	ULONG           HighWaterNumberOfHandles;
	ULONG           HighWaterPagedPoolUsage;
	ULONG           HighWaterNonPagedPoolUsage;
	ULONG           HighWaterNamePoolUsage;
	ULONG           HighWaterHandleTableUsage;
	ULONG           InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG           ValidAccess;
	BOOLEAN         SecurityRequired;
	BOOLEAN         MaintainHandleCount;
	USHORT          MaintainTypeList;
	POOL_TYPE       PoolType;
	ULONG           PagedPoolUsage;
	ULONG           NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFO {
	ULONG          NextEntryOffset;
	ULONG          NumberOfThreads;
	LARGE_INTEGER  Reserved[3];
	LARGE_INTEGER  CreateTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  KernelTime;
	UNICODE_STRING ImageName;
	ULONG          BasePriority;
	HANDLE         ProcessId;
	HANDLE         InheritedFromProcessId;
} SYSTEM_PROCESS_INFO, * PSYSTEM_PROCESS_INFO;

using SECTION_INHERIT = enum _SECTION_INHERIT {
	ViewShare = 1,
	ViewUnmap = 2
};

typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInformation,
	SectionImageInformation
} SECTION_INFORMATION_CLASS, * PSECTION_INFORMATION_CLASS;

typedef struct _SECTION_IMAGE_INFORMATION {
	PVOID TransferAddress;
	ULONG ZeroBits;
	SIZE_T MaximumStackSize;
	SIZE_T CommittedStackSize;
	ULONG SubSystemType;
	union
	{
		struct
		{
			USHORT SubSystemMinorVersion;
			USHORT SubSystemMajorVersion;
		};
		ULONG SubSystemVersion;
	};
	union
	{
		struct
		{
			USHORT MajorOperatingSystemVersion;
			USHORT MinorOperatingSystemVersion;
		};
		ULONG OperatingSystemVersion;
	};
	USHORT ImageCharacteristics;
	USHORT DllCharacteristics;
	USHORT Machine;
	BOOLEAN ImageContainsCode;
	union
	{
		UCHAR ImageFlags;
		struct
		{
			UCHAR ComPlusNativeReady : 1;
			UCHAR ComPlusILOnly : 1;
			UCHAR ImageDynamicallyRelocated : 1;
			UCHAR ImageMappedFlat : 1;
			UCHAR BaseBelow4gb : 1;
			UCHAR ComPlusPrefer32bit : 1;
			UCHAR Reserved : 2;
		};
	};
	ULONG LoaderFlags;
	ULONG ImageFileSize;
	ULONG CheckSum;
} SECTION_IMAGE_INFORMATION, * PSECTION_IMAGE_INFORMATION;

typedef struct _SECTION_BASIC_INFORMATION {
	PVOID         BaseAddress;
	ULONG         AllocationAttributes;
	LARGE_INTEGER MaximumSize;
} SECTION_BASIC_INFORMATION, * PSECTION_BASIC_INFORMATION;

typedef _CLIENT_ID *PCLIENT_ID;

typedef struct _INITIAL_TEB {
	struct {
		PVOID OldStackBase;
		PVOID OldStackLimit;
	} OldInitialTeb;
	PVOID StackBase;
	PVOID StackLimit;
	PVOID StackAllocationBase;
} INITIAL_TEB, * PINITIAL_TEB;

using ZwMapViewOfSectionProc    = DWORD(WINAPI*)(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect);
using NTQUERYINFORMATIONPROCESS = NTSTATUS(__stdcall*)(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG_PTR ReturnLength);
using _NtQuerySection           = NTSTATUS(NTAPI*)(_In_ HANDLE SectionHandle, _In_ SECTION_INFORMATION_CLASS SectionInformationClass, _Out_writes_bytes_(SectionInformationLength) PVOID SectionInformation, _In_ SIZE_T SectionInformationLength, _Out_opt_ PSIZE_T ReturnLength);
using _ZwCreateSection          = DWORD(WINAPI*)(PHANDLE SectionHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PLARGE_INTEGER MaximumSize, ULONG SectionPageProtection, ULONG AllocationAttributes, HANDLE FileHandle);
using _ZwMapViewOfSection       = DWORD(WINAPI*)(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect);
using _NtDuplicateObject        = NTSTATUS(NTAPI*)(HANDLE SourceProcessHandle, HANDLE SourceHandle, HANDLE TargetProcessHandle, PHANDLE TargetHandle, ACCESS_MASK DesiredAccess, ULONG Attributes, ULONG Options);
using _NtQueryObject            = NTSTATUS(NTAPI*)(HANDLE ObjectHandle, ULONG ObjectInformationClass, PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength);
using _NtUnmapViewOfSection     = NTSTATUS(NTAPI*)(HANDLE ProcessHandle, PVOID BaseAddress);

extern std::function<void(PCONTEXT, _EXCEPTION_RECORD*)> RestoreContext;
extern ULONG myModuleSize;
extern _NtQuerySection NtQuerySection;
extern _ZwCreateSection ZwCreateSection;
extern _ZwMapViewOfSection ZwMapViewOfSection;
extern _NtUnmapViewOfSection NtUnmapViewOfSection;

extern DWORD GetProcessIdByName(const std::wstring& name);
