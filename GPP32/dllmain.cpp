#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <thread>
#include "main/main.h"

static HMODULE gHModule;
static DWORD crc32;

auto CheckMemory() -> DWORD;

auto APIENTRY DllMain(const HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) -> BOOL {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			gHModule = hModule;
            if (GetModuleHandle(L"Sausage Man.exe")) {
                std::thread(Main::Init, hModule).detach();
            }
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH: break;
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

auto GetIdByName(const std::wstring& name) -> DWORD {
	const HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE) return -1;

	PROCESSENTRY32 processer;
	processer.dwSize = sizeof(PROCESSENTRY32);

	int flag = Process32First(hsnapshot, &processer);
	while (flag != 0) {
		if (processer.szExeFile == name) return processer.th32ProcessID;
		flag = Process32Next(hsnapshot, &processer);
	}

	CloseHandle(hsnapshot);
	return -2;
}

static DWORD idThread;

auto CALLBACK EnumFunc(const HWND hWnd, const LPARAM pid) -> BOOL {
	DWORD dwProcessId;

	dwProcessId                       = 0;
	const DWORD windowThreadProcessId = GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId != pid || !IsWindowVisible(hWnd)) return 1;
	idThread = windowThreadProcessId;
	return 0;
}

extern "C" _declspec(dllexport) auto Inject() -> void {
	if (const auto pid = GetIdByName(L"Sausage Man.exe")) {
		EnumWindows(EnumFunc, pid);
		if (idThread) if (SetWindowsHookExW(WH_GETMESSAGE, HookCallBack, gHModule, idThread)) PostThreadMessageW(idThread, WM_NULL, 0, 0);
	}
}
