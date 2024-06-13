#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <print>
#include <shlwapi.h>
#include <string>
#include <thread>
#include "../cheat/main.h"

#include "Base/ResourceLoader.h"
#include "Base/cheat/misc/Settings.h"
#include "Base/inject/load-library.h"

#define GAME_NAME L"Sausage Man.exe"
#define LAUNCHER_NAME "Loader.exe"

static HMODULE gHModule;
static CSimpleIniA ini;

auto WINAPI MyCreateProcessW(_In_opt_ const LPCWSTR lpApplicationName, _Inout_opt_ const LPWSTR lpCommandLine, _In_opt_ const LPSECURITY_ATTRIBUTES lpProcessAttributes, _In_opt_ const LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ const BOOL bInheritHandles, _In_ DWORD dwCreationFlags, _In_opt_ const LPVOID lpEnvironment, _In_opt_ const LPCWSTR lpCurrentDirectory, _In_ const LPSTARTUPINFOW lpStartupInfo, _Out_ const LPPROCESS_INFORMATION lpProcessInformation) -> BOOL {
	dwCreationFlags |= CREATE_SUSPENDED;

	if (!IsBadStringPtrW(lpApplicationName, MAX_PATH))
		LOG_TRACE("CreateProcessW File: %s", encode::w2s(lpApplicationName).data());

	const auto ret = CALL_ORIGIN(MyCreateProcessW, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

	char pathOut[MAX_PATH] = {};
	GetModuleFileNameA(gHModule, pathOut, MAX_PATH);

	LoadLibraryDLL(lpProcessInformation->hProcess, pathOut);
	Sleep(2000);
	ResumeThread(lpProcessInformation->hThread);
	return ret;
}

auto WINAPI MyCreateProcessA(_In_opt_ const LPCSTR lpApplicationName, _Inout_opt_ const LPSTR lpCommandLine, _In_opt_ const LPSECURITY_ATTRIBUTES lpProcessAttributes, _In_opt_ const LPSECURITY_ATTRIBUTES lpThreadAttributes, _In_ const BOOL bInheritHandles, _In_ DWORD dwCreationFlags, _In_opt_ const LPVOID lpEnvironment, _In_opt_ const LPCSTR lpCurrentDirectory, _In_ const LPSTARTUPINFOA lpStartupInfo, _Out_ const LPPROCESS_INFORMATION lpProcessInformation) -> BOOL {
	dwCreationFlags |= CREATE_SUSPENDED;

	if (!IsBadStringPtrA(lpApplicationName, MAX_PATH))
		LOG_TRACE("CreateProcessA File: %s", lpApplicationName);

	const auto ret = CALL_ORIGIN(MyCreateProcessA, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);

	char pathOut[MAX_PATH] = {};
	GetModuleFileNameA(gHModule, pathOut, MAX_PATH);

	LoadLibraryDLL(lpProcessInformation->hProcess, pathOut);
	Sleep(2000);
	ResumeThread(lpProcessInformation->hThread);
	return ret;
}

auto WINAPI MyCreateServiceA(_In_ const SC_HANDLE hSCManager, _In_ const LPCSTR lpServiceName, _In_opt_ const LPCSTR lpDisplayName, _In_ const DWORD dwDesiredAccess, _In_ const DWORD dwServiceType, _In_ const DWORD dwStartType, _In_ const DWORD dwErrorControl, _In_opt_ LPCSTR lpBinaryPathName, _In_opt_ const LPCSTR lpLoadOrderGroup, _Out_opt_ const LPDWORD lpdwTagId, _In_opt_ const LPCSTR lpDependencies, _In_opt_ const LPCSTR lpServiceStartName, _In_opt_ const LPCSTR lpPassword) -> SC_HANDLE {
	LOG_CRIT("CreateServiceA 驱动拦截成功");
	char pathOut[MAX_PATH] = {};
	GetModuleFileNameA(gHModule, pathOut, MAX_PATH);
	const std::filesystem::path _currentPath = pathOut;
	return CALL_ORIGIN(MyCreateServiceA, hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType, dwStartType, dwErrorControl, static_cast<LPCSTR>((_currentPath / "Driver.sys").string().data()), lpLoadOrderGroup, lpdwTagId, lpDependencies, lpServiceStartName, lpPassword);
}

auto WINAPI MyCreateServiceW(_In_ const SC_HANDLE hSCManager, _In_ const LPCWSTR lpServiceName, _In_opt_ const LPCWSTR lpDisplayName, _In_ const DWORD dwDesiredAccess, _In_ const DWORD dwServiceType, _In_ const DWORD dwStartType, _In_ const DWORD dwErrorControl, _In_opt_ LPCWSTR lpBinaryPathName, _In_opt_ const LPCWSTR lpLoadOrderGroup, _Out_opt_ const LPDWORD lpdwTagId, _In_opt_ const LPCWSTR lpDependencies, _In_opt_ const LPCWSTR lpServiceStartName, _In_opt_ const LPCWSTR lpPassword) -> SC_HANDLE {
	LOG_CRIT("CreateServiceW 驱动拦截成功");
	static std::filesystem::path _currentPath;
	char pathOut[MAX_PATH] = {};
	GetModuleFileNameA(gHModule, pathOut, MAX_PATH);
	_currentPath = std::filesystem::path(pathOut).parent_path().string();
	return CALL_ORIGIN(MyCreateServiceW, hSCManager, lpServiceName, lpDisplayName, dwDesiredAccess, dwServiceType, dwStartType, dwErrorControl, static_cast<LPCWSTR>(encode::s2w((_currentPath / "Driver.sys").string()).data()), lpLoadOrderGroup, lpdwTagId, lpDependencies, lpServiceStartName, lpPassword);
}

auto WINAPI MyIsDebuggerPresent() -> BOOL { return false; }

auto WINAPI MyGetShellWindow() -> HWND { return nullptr; }


auto WINAPI MyK32EnumProcessModules(_In_ HANDLE hProcess, _Out_writes_bytes_(cb) HMODULE* lphModule, _In_ DWORD cb, _Out_ const LPDWORD lpcbNeeded) -> BOOL {
	*lpcbNeeded = 0;
	return true;
}

auto WINAPI MyK32EnumProcessModulesEx(_In_ HANDLE hProcess, _Out_writes_bytes_(cb) HMODULE* lphModule, _In_ DWORD cb, _Out_ const LPDWORD lpcbNeeded, _In_ DWORD dwFilterFlag) -> BOOL {
	*lpcbNeeded = 0;
	return true;
}

auto WINAPI MyCreateToolhelp32Snapshot(DWORD dwFlags, const DWORD th32ProcessID) -> HANDLE { return CALL_ORIGIN(MyCreateToolhelp32Snapshot, (DWORD)TH32CS_SNAPPROCESS, th32ProcessID); }

auto WINAPI MyModule32First(const HANDLE hSnapshot, const LPMODULEENTRY32W lpme) -> BOOL {
	const auto ret = CALL_ORIGIN(MyModule32First, hSnapshot, lpme);
	if (lpme->hModule == gHModule) return false;
	return ret;
}

auto WINAPI MyModule32Next(const HANDLE hSnapshot, const LPMODULEENTRY32W lpme) -> BOOL {
	const auto ret = CALL_ORIGIN(MyModule32First, hSnapshot, lpme);
	if (lpme->hModule == gHModule) return CALL_ORIGIN(MyModule32First, hSnapshot, lpme);
	return ret;
}

auto WINAPI MyThread32First(HANDLE hSnapshot, LPTHREADENTRY32 lpte) -> BOOL { return false; }

auto WINAPI MyThread32Next(HANDLE hSnapshot, LPTHREADENTRY32 lpte) -> BOOL { return false; }

auto WINAPI MyHeap32First(LPHEAPENTRY32 lphe, DWORD th32ProcessID, ULONG_PTR th32HeapID) -> BOOL { return false; }

auto WINAPI MyHeap32Next(LPHEAPENTRY32 lphe) -> BOOL { return false; }

auto WINAPI MyHeap32ListFirst(HANDLE hSnapshot, LPHEAPLIST32 lphl) -> BOOL { return false; }

auto WINAPI MyHeap32ListNext(HANDLE hSnapshot, LPHEAPLIST32 lphl) -> BOOL { return false; }

HANDLE driver;

auto WINAPI MyCreateFileA(_In_ const LPCSTR lpFileName, _In_ const DWORD dwDesiredAccess, _In_ const DWORD dwShareMode, _In_opt_ const LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ const DWORD dwCreationDisposition, _In_ const DWORD dwFlagsAndAttributes, _In_opt_ const HANDLE hTemplateFile) -> HANDLE {
	if (!IsBadStringPtrA(lpFileName, MAX_PATH)) {
	//	LOG_TRACE("CreateFileA : %s", lpFileName);
	//	if (!driver && std::string(lpFileName).find("XMFunny") != std::string::npos) return driver = CALL_ORIGIN(MyCreateFileA, lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		if (std::string(lpFileName).find("GPP32.dll") != std::string::npos) return nullptr;
	//	if (std::string(lpFileName).find("Driver.sys") != std::string::npos) return nullptr;
	}
	return CALL_ORIGIN(MyCreateFileA, lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

auto WINAPI MyCreateFileW(_In_ const LPCWSTR lpFileName, _In_ const DWORD dwDesiredAccess, _In_ const DWORD dwShareMode, _In_opt_ const LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ const DWORD dwCreationDisposition, _In_ const DWORD dwFlagsAndAttributes, _In_opt_ const HANDLE hTemplateFile) -> HANDLE {
	if (!IsBadStringPtrW(lpFileName, MAX_PATH)) {
	//	LOG_TRACE("CreateFileW : %s", encode::w2s(lpFileName).data());
	//	if (!driver && std::string(encode::w2s(lpFileName)).find("XMFunny") != std::string::npos) return driver = CALL_ORIGIN(MyCreateFileW, lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		if (std::string(encode::w2s(lpFileName)).find("GPP32.dll") != std::string::npos) return nullptr;
	//	if (std::string(encode::w2s(lpFileName)).find("Driver.sys") != std::string::npos) return nullptr;
	}
	return CALL_ORIGIN(MyCreateFileW, lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

auto WINAPI MyDeviceIoControl(_In_ const HANDLE hDevice, _In_ const DWORD dwIoControlCode, _In_reads_bytes_opt_(nInBufferSize) const LPVOID lpInBuffer, _In_ const DWORD nInBufferSize, _Out_writes_bytes_to_opt_(nOutBufferSize, *lpBytesReturned) const LPVOID lpOutBuffer, _In_ const DWORD nOutBufferSize, _Out_opt_ const LPDWORD lpBytesReturned, _Inout_opt_ const LPOVERLAPPED lpOverlapped) -> BOOL {
	if (hDevice == driver) {
		LOG_CRIT("DeviceIoControl Driver");
		return true;
	}
	return CALL_ORIGIN(MyDeviceIoControl, hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
}

auto WINAPI MyZwCreateSection(PHANDLE SectionHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PLARGE_INTEGER MaximumSize, ULONG SectionPageProtection, ULONG AllocationAttributes, HANDLE FileHandle) -> DWORD {
	if (AllocationAttributes == 138412032) {
		DesiredAccess = SECTION_ALL_ACCESS;
		SectionPageProtection = PAGE_EXECUTE_READWRITE;
		AllocationAttributes = SEC_COMMIT;
		LOG_CRIT("重映射处理完成!");
	}
	const auto ret = CALL_ORIGIN(MyZwCreateSection, SectionHandle, DesiredAccess, ObjectAttributes, MaximumSize, SectionPageProtection, AllocationAttributes, FileHandle);
	return ret;
}

auto WINAPI MyZwMapViewOfSection(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect) -> DWORD {
	auto ret = CALL_ORIGIN(MyZwMapViewOfSection, SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Win32Protect);
	MEMORY_BASIC_INFORMATION memInfo = {nullptr};
	VirtualQuery(*BaseAddress, &memInfo, sizeof(MEMORY_BASIC_INFORMATION));
	if (memInfo.State == MEM_COMMIT && memInfo.Type == MEM_MAPPED && memInfo.RegionSize >= 60000000 && memInfo.RegionSize <= 80000000 && memInfo.Protect == PAGE_EXECUTE_READ) {
		NtUnmapViewOfSection(GetCurrentProcess(), *BaseAddress);
		ret = CALL_ORIGIN(MyZwMapViewOfSection, SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, (ULONG)PAGE_EXECUTE_READWRITE);
	}
	return ret;
}

auto NTAPI MyNtQuerySection(_In_ HANDLE SectionHandle, _In_ SECTION_INFORMATION_CLASS SectionInformationClass, _Out_writes_bytes_(SectionInformationLength) PVOID SectionInformation, _In_ SIZE_T SectionInformationLength, _Out_opt_ PSIZE_T ReturnLength) -> NTSTATUS { return CALL_ORIGIN(MyNtQuerySection, SectionHandle, SectionInformationClass, SectionInformation, SectionInformationLength, ReturnLength); }

auto WINAPI MyVirtualQuery(_In_opt_ LPCVOID lpAddress, _Out_writes_bytes_to_(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer, _In_ SIZE_T dwLength) -> SIZE_T {
	if (reinterpret_cast<std::uintptr_t>(lpAddress) >= reinterpret_cast<std::uintptr_t>(gHModule) && reinterpret_cast<std::uintptr_t>(lpAddress) <= reinterpret_cast<std::uintptr_t>(gHModule) + static_cast<std::uintptr_t>(myModuleSize)) return 0;
	return CALL_ORIGIN(MyVirtualQuery, lpAddress, lpBuffer, dwLength);
}

auto WINAPI MyReadProcessMemory(_In_ HANDLE hProcess, _In_ LPCVOID lpBaseAddress, _Out_writes_bytes_to_(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer, _In_ SIZE_T nSize, _Out_opt_ SIZE_T* lpNumberOfBytesRead) -> BOOL {
	if (reinterpret_cast<std::uintptr_t>(lpBaseAddress) >= reinterpret_cast<std::uintptr_t>(gHModule) && reinterpret_cast<std::uintptr_t>(lpBaseAddress) <= reinterpret_cast<std::uintptr_t>(gHModule) + static_cast<std::uintptr_t>(myModuleSize)) return FALSE;
	return CALL_ORIGIN(MyReadProcessMemory, hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

auto APIENTRY MyGetConsoleWindow(VOID) -> HWND { return nullptr; }

std::mutex* mutex = new std::mutex();
WNDENUMPROC wndenumproc;

auto CALLBACK MyWndProc(HWND hwnd, LPARAM lParam) -> BOOL {
	if (hwnd == CALL_ORIGIN(MyGetConsoleWindow)) return true;
	return wndenumproc(hwnd, lParam);
}

auto WINAPI MyEnumWindows(_In_ WNDENUMPROC lpEnumFunc, _In_ LPARAM lParam) -> BOOL {
	mutex->lock();
	wndenumproc = lpEnumFunc;
	const auto ret = CALL_ORIGIN(MyEnumWindows, MyWndProc, lParam);
	mutex->unlock();
	return ret;
}

auto APIENTRY DllMain(const HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) -> BOOL {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			gHModule = hModule;

			DWORD pPEB;
			PPEB_LDR_DATA pLDR;

			__asm {
				mov eax, fs: [0x30]
				mov pPEB, eax
				mov eax, [eax + 0x0C]
				mov pLDR, eax
			}

			PLIST_ENTRY pMiddle = &(pLDR->InMemoryOrderModuleList);
			PLIST_ENTRY pNext = pMiddle->Flink;

			do {
				auto pLDTE = CONTAINING_RECORD(pNext, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
				if (gHModule == pLDTE->DllBase) {
					pLDTE->InMemoryOrderLinks.Blink->Flink = pLDTE->InMemoryOrderLinks.Flink;
					pLDTE->InMemoryOrderLinks.Flink->Blink = pLDTE->InMemoryOrderLinks.Blink;

					pLDTE->InMemoryOrderLinks.Blink->Flink = pLDTE->InMemoryOrderLinks.Flink;
					pLDTE->InMemoryOrderLinks.Flink->Blink = pLDTE->InMemoryOrderLinks.Blink;

					pLDTE->InMemoryOrderLinks.Blink->Flink = pLDTE->InMemoryOrderLinks.Flink;
					pLDTE->InMemoryOrderLinks.Flink->Blink = pLDTE->InMemoryOrderLinks.Blink;

					break;
				}
				pNext = pNext->Flink;
			} while (pMiddle != pNext);

			myModuleSize = DetourGetModuleSize(gHModule);

			if (const auto hmodule = GetModuleHandleA("ntdll.dll")) {
				RestoreContext = reinterpret_cast<void(*)(PCONTEXT, _EXCEPTION_RECORD*)>(GetProcAddress(hmodule, "RtlRestoreContext"));
				NtQuerySection = reinterpret_cast<_NtQuerySection>(GetProcAddress(hmodule, "NtQuerySection"));
				ZwCreateSection = reinterpret_cast<_ZwCreateSection>(GetProcAddress(hmodule, "NtCreateSection"));
				ZwMapViewOfSection = reinterpret_cast<_ZwMapViewOfSection>(GetProcAddress(hmodule, "ZwMapViewOfSection"));
				NtUnmapViewOfSection = reinterpret_cast<_NtUnmapViewOfSection>(GetProcAddress(hmodule, "NtUnmapViewOfSection"));
			}

			ResourceLoader::SetModuleHandle(gHModule);
			util::SetCurrentPath(util::GetModulePath(gHModule));

			config::Initialize((util::GetCurrentPath() / "cfg.json").string());

			Logger::SetLevel(Logger::Level::Trace, Logger::LoggerType::Any);
			const auto settings = cheat::feature::Settings::GetInstance();
			if (settings->f_FileLogging) Logger::PrepareFileLogging((util::GetCurrentPath() / "logs").string());

			if (settings->f_ConsoleLogging) {
				AllocConsole();
				freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
				freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
				system("chcp 65001");
			}

			MH_Initialize();
			veh::Setup();
			VEH::Init();

			if (GetModuleHandle(L"Launcher.exe")) return TRUE;
			//HookManager::install(ZwCreateSection, MyZwCreateSection);
			//HookManager::install(ZwMapViewOfSection, MyZwMapViewOfSection);
			//HookManager::install(CreateProcessW, MyCreateProcessW);
			//HookManager::install(CreateProcessA, MyCreateProcessA);
			//HookManager::install(CreateServiceA, MyCreateServiceA);
			//HookManager::install(CreateServiceW, MyCreateServiceW);
			HookManager::install(IsDebuggerPresent, MyIsDebuggerPresent);
			HookManager::install(CreateToolhelp32Snapshot, MyCreateToolhelp32Snapshot);
			HookManager::install(Module32First, MyModule32First);
			HookManager::install(Module32Next, MyModule32Next);
			HookManager::install(K32EnumProcessModules, MyK32EnumProcessModules);
			HookManager::install(K32EnumProcessModulesEx, MyK32EnumProcessModulesEx);
			HookManager::install(Thread32First, MyThread32First);
			HookManager::install(Thread32Next, MyThread32Next);
			HookManager::install(Thread32Next, MyThread32Next);
			HookManager::install(Heap32First, MyHeap32First);
			HookManager::install(Heap32Next, MyHeap32Next);
			HookManager::install(Heap32ListFirst, MyHeap32ListFirst);
			HookManager::install(Heap32ListNext, MyHeap32ListNext);
			HookManager::install(GetShellWindow, MyGetShellWindow);
			HookManager::install(NtQuerySection, MyNtQuerySection);
			HookManager::install(VirtualQuery, MyVirtualQuery);
			HookManager::install(ReadProcessMemory, MyReadProcessMemory);
			HookManager::install(EnumWindows, MyEnumWindows);
			HookManager::install(GetConsoleWindow, MyGetConsoleWindow);

			if (GetModuleHandle(GAME_NAME)) {
				//HookManager::install(CreateFileA, MyCreateFileA);
				//HookManager::install(CreateFileW, MyCreateFileW);
				//HookManager::install(DeviceIoControl, MyDeviceIoControl);
				std::thread(Main::Run, gHModule).detach();
			}
			break;
		}
		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
		case DLL_PROCESS_DETACH: {
			LOG_INFO("进程将在5s后退出...");
			Sleep(5000);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
	return TRUE;
}

auto CALLBACK HookCallBack(const int Code, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
	std::wstring file;
	file.resize(255, '\0');
	GetModuleFileNameW(gHModule, file.data(), 255);
	LoadLibraryW(file.c_str());
	return CallNextHookEx(nullptr, Code, wParam, lParam);
}

static std::vector<DWORD> pids;

auto GetProcessIdByName(const std::wstring& name) -> DWORD {
	const HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE) return -1;

	PROCESSENTRY32 processer;
	processer.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &processer);
	while (flag != 0) {
		if (strcmp(encode::w2s(name).data(), encode::w2s(processer.szExeFile).data()) == 0) pids.push_back(processer.th32ProcessID);
		flag = Process32Next(hsnapshot, &processer);
	}

	CloseHandle(hsnapshot);
	return -2;
}

static std::vector<DWORD> idThread;

auto CALLBACK EnumFunc(const HWND hWnd, const LPARAM pid) -> BOOL {
	DWORD dwProcessId;

	dwProcessId = 0;
	const DWORD windowThreadProcessId = GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId != pid || !IsWindowVisible(hWnd)) return 1;
	idThread.push_back(windowThreadProcessId);
	return 0;
}

extern "C" _declspec(dllexport) auto WindowsMessageInject() -> int {
	GetProcessIdByName(GAME_NAME);
	for (const auto pid : pids) {
		LOG_TRACE("PID: %p", pid);
		idThread.clear();
		EnumWindows(EnumFunc, pid);
		for (const auto id : idThread) {
			LOG_TRACE("--Thread ID: %p", id);
			if (SetWindowsHookExW(WH_GETMESSAGE, HookCallBack, gHModule, id)) {

				PostThreadMessageW(id, WM_NULL, 0, 0);
			}
		}
	}
	return true;
}

auto FindProcessId(const std::wstring& processName) -> int {
	int pid = -1;

	const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	if (Process32First(snapshot, &process)) {
		do {
			if (strcmp(encode::w2s(processName).data(), encode::w2s(process.szExeFile).data()) == 0) {
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	return pid;
}

auto OpenTargetProcess(HANDLE* phProcess, HANDLE* phThread) -> bool {
	const DWORD pid = FindProcessId(L"explorer.exe");
	if (pid == 0) {
		printf("找不到 'explorer.exe' 进程ID!\n");
		return false;
	}

	HANDLE hToken;
	const BOOL TokenRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
	if (!TokenRet) {
		printf("权限提升失败!\n");
		return false;
	}

	auto filePath = util::GetOrSelectPath(ini, "Inject", "path", LAUNCHER_NAME" 路径", LAUNCHER_NAME "\0" LAUNCHER_NAME "\0");
	const auto commandline = ini.GetValue("Inject", "CommandLine");

	const LPSTR lpstr = commandline == nullptr ? nullptr : const_cast<LPSTR>(commandline);
	if (!filePath) return false;

	std::string CurrentDirectory = filePath.value();
	const int pos = CurrentDirectory.rfind("\\", CurrentDirectory.length());
	CurrentDirectory = CurrentDirectory.substr(0, pos);

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	SIZE_T lpsize = 0;
	InitializeProcThreadAttributeList(nullptr, 1, 0, &lpsize);

	const auto temp = new char[lpsize];
	const auto AttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(temp);
	InitializeProcThreadAttributeList(AttributeList, 1, 0, &lpsize);
	if (!UpdateProcThreadAttribute(AttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &handle, sizeof(HANDLE), nullptr, nullptr)) printf("UpdateProcThreadAttribute failed ! (%d).\n", GetLastError());

	STARTUPINFOEXA si{};
	si.StartupInfo.cb = sizeof(si);
	si.lpAttributeList = AttributeList;

	PROCESS_INFORMATION pi{};
	const BOOL result = CreateProcessAsUserA(hToken, filePath->data(), lpstr, nullptr, nullptr, 0, CREATE_SUSPENDED, nullptr, CurrentDirectory.data(), reinterpret_cast<LPSTARTUPINFOA>(&si), &pi);

	const bool isOpened = result;
	if (isOpened) {
		ini.SaveFile("cfg.ini");
		*phThread = pi.hThread;
		*phProcess = pi.hProcess;
	} else {
		printf("无法创建进程.\n");
		printf("如果 " LAUNCHER_NAME " 路径有问题。您可以在 cfg.ini 中手动更改它.\n");
	}

	DeleteProcThreadAttributeList(AttributeList);
	delete[] temp;
	return isOpened;
}

extern "C" _declspec(dllexport) auto ProcessInterceptionInject() -> int {
	const auto path = std::filesystem::path(util::GetModulePath(gHModule)).parent_path();

	ini.SetUnicode();
	ini.LoadFile("cfg.ini");

	HANDLE hProcess, hThread;
	if (!OpenTargetProcess(&hProcess, &hThread)) {
		std::cout << "打开进程失败." << std::endl;
		system("pause");
		return false;
	}

	LoadLibraryDLL(hProcess, (path / "GPP32.dll").string());

	Sleep(1000);
	ResumeThread(hThread);
	CloseHandle(hProcess);
	return true;
}
