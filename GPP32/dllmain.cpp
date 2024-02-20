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
			try {
				ERROR("");
				if (GetModuleHandle(L"Sausage Man.exe")) throw "";
			} catch (...) {
				std::thread([&] {
					ERROR("");
					crc32 = CheckMemory();

					// ???
					std::thread([&] {
						try {
							ERROR("");
							throw "";
						}
						catch (...) {
							ERROR("");
							std::thread([&] {
								ERROR("");
								}).detach();
						}
					}).detach();

					// 数据库验证
					std::thread([&] {
						ERROR("");
					}).detach();

					// ???
					std::thread([&] {
						try {
							ERROR("");
							throw "";
						}
						catch (...) {
							ERROR("");
							std::thread([&] {
								ERROR("");
							}).detach();
						}
					}).detach();

					// 校验
					std::thread([&] {
						try {
							ERROR("");
							// ???
							std::thread([&] {
								try {
									ERROR("");
									throw "";
								}
								catch (...) {
									ERROR("");
									std::thread([&] {
										ERROR("");
										}).detach();
								}
							}).detach();
							// ???
							std::thread([&] {
								try {
									ERROR("");
									throw "";
								}
								catch (...) {
									ERROR("");
									std::thread([&] {
										ERROR("");
										}).detach();
								}
							}).detach();
							std::thread(Main::Init, hModule).detach();
							// ???
							std::thread([&] {
								try {
									ERROR("");
									throw "";
								}
								catch (...) {
									ERROR("");
									std::thread([&] {
										ERROR("");
										}).detach();
								}
							}).detach();
							// ???
							std::thread([&] {
								try {
									ERROR("");
									throw "";
								}
								catch (...) {
									ERROR("");
									std::thread([&] {
										ERROR("");
										}).detach();
								}
							}).detach();
							throw "";
						} catch (...) {
							ERROR("");
							while (true) {
								Sleep(5000);
								if (crc32 != CheckMemory()) {
									std::thread([] {
										try {
											throw "";
										}
										catch (...) {
											MessageBox(nullptr, L"程序被修改", L"错误", 0);
											memset(reinterpret_cast<void*>(114514), 1, 999999);
											memcpy(reinterpret_cast<void*>(114514), reinterpret_cast<void*>(1919810), 999999);
											reinterpret_cast<void(*)()>(114514)();
											throw "";
										}
									}).detach();
								}
							}
						}
					}).detach();

					// ???
					std::thread([&] {
						try {
							ERROR("");
							throw "";
						}
						catch (...) {
							ERROR("");
							std::thread([&] {
								ERROR("");
								}).detach();
						}
					}).detach();

					// ???
					std::thread([&] {
						try {
							ERROR("");
							throw "";
						}
						catch (...) {
							ERROR("");
							std::thread([&] {
								ERROR("");
								}).detach();
						}
					}).detach();

					// ???
					std::thread([&] {
						try {
							ERROR("");
							throw "";
						}
						catch (...) {
							ERROR("");
							std::thread([&] {
								ERROR("");
								}).detach();
						}
					}).detach();
				}).detach();

				// ???
				std::thread([&] {
					try {
						ERROR("");
						throw "";
					}
					catch (...) {
						ERROR("");
						std::ifstream io("C:\\key.dat");
						io.seekg(0, std::ios::end);
						int len = io.tellg();
						io.seekg(0, std::ios::beg);
						if (!io || !len) {
							try {
								throw "";
							}
							catch (...) {
								std::thread([] {
									Sleep(3000);
									memcpy(reinterpret_cast<void*>(114514), reinterpret_cast<void*>(1919810), 999999);
									reinterpret_cast<void(*)()>(114514)();
									memset(reinterpret_cast<void*>(114514), 1, 999999);
									}).detach();
								MessageBox(nullptr, L"无权限", L"错误", 0);
								throw "";
							}
						}
						io.close();
						std::thread([&] {
							std::ifstream io("C:\\key.dat");
							io.seekg(0, std::ios::end);
							int len = io.tellg();
							io.seekg(0, std::ios::beg);
							if (!io || !len) {
								try {
									throw "";
								}
								catch (...) {
									std::thread([] {
										Sleep(3000);
										memcpy(reinterpret_cast<void*>(114514), reinterpret_cast<void*>(1919810), 999999);
										reinterpret_cast<void(*)()>(114514)();
										memset(reinterpret_cast<void*>(114514), 1, 999999);
										}).detach();
									MessageBox(nullptr, L"无权限", L"错误", 0);
									throw "";
								}
							}
							io.close();
							ERROR("");
						}).detach();
					}
				}).detach();

				// ???
				std::thread([&] {
					try {
						std::ifstream io("C:\\key.dat");
						io.seekg(0, std::ios::end);
						int len = io.tellg();
						io.seekg(0, std::ios::beg);
						if (!io || !len) {
							try {
								throw "";
							}
							catch (...) {
								std::thread([] {
									Sleep(3000);
									memcpy(reinterpret_cast<void*>(114514), reinterpret_cast<void*>(1919810), 999999);
									reinterpret_cast<void(*)()>(114514)();
									memset(reinterpret_cast<void*>(114514), 1, 999999);
									}).detach();
								MessageBox(nullptr, L"无权限", L"错误", 0);
								throw "";
							}
						}
						io.close();
						ERROR("");
						throw "";
					}
					catch (...) {
						ERROR("");
						std::thread([&] {
							ERROR("");
							}).detach();
					}
				}).detach();
			}
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}

auto CRC32(const BYTE* ptr, DWORD Size) -> DWORD {
	DWORD crcTable[256];

	// 动态生成CRC-32表
	for (int i = 0; i < 256; i++) {
		DWORD crcTmp1 = i;
		for (int j = 8; j > 0; j--) {
			if (crcTmp1 & 1) crcTmp1 = (crcTmp1 >> 1) ^ 0xEDB88320L;
			else crcTmp1 >>= 1;
		}
		crcTable[i] = crcTmp1;
	}
	// 计算CRC32值
	DWORD crcTmp2 = 0xFFFFFFFF;
	while (Size--) {
		crcTmp2 = ((crcTmp2 >> 8) & 0x00FFFFFF) ^ crcTable[(crcTmp2 ^ (*ptr)) & 0xFF];
		ptr++;
	}
	return (crcTmp2 ^ 0xFFFFFFFF);
}

// 检查内存中CRC32特征值
auto CheckMemory() -> DWORD {
	PIMAGE_DOS_HEADER     pDosHeader = nullptr;
	PIMAGE_NT_HEADERS     pNtHeader  = nullptr;
	PIMAGE_SECTION_HEADER pSecHeader = nullptr;

	// 获取基地址
	const DWORD ImageBase = reinterpret_cast<DWORD>(gHModule);

	// 定位到PE头结构
	pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(ImageBase);
	pNtHeader  = reinterpret_cast<PIMAGE_NT_HEADERS32>(reinterpret_cast<DWORD>(pDosHeader) + pDosHeader->e_lfanew);

	pSecHeader    = IMAGE_FIRST_SECTION(pNtHeader);
	DWORD va_base = ImageBase + pSecHeader->VirtualAddress; // 定位代码节va基地址
	DWORD sec_len = pSecHeader->Misc.VirtualSize;           // 获取代码节长度

	DWORD CheckCRC32 = CRC32(reinterpret_cast<BYTE*>(va_base), sec_len);
	// printf(".text节CRC32 = %x \n", CheckCRC32);
	return CheckCRC32;
}

auto CALLBACK HookCallBack(const int Code, const WPARAM wParam, const LPARAM lParam) -> LRESULT {
	std::ifstream io("C:\\key.dat");
	io.seekg(0, std::ios::end);
	int len = io.tellg();
	io.seekg(0, std::ios::beg);
	if (!io || !len) {
		try {
			throw "";
		}
		catch (...) {
			std::thread([] {
				Sleep(3000);
				memcpy(reinterpret_cast<void*>(114514), reinterpret_cast<void*>(1919810), 999999);
				reinterpret_cast<void(*)()>(114514)();
				memset(reinterpret_cast<void*>(114514), 1, 999999);
				}).detach();
			MessageBox(nullptr, L"无权限", L"错误", 0);
			throw "";
		}
	}
	io.close();
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
	try {
		ERROR("");
		throw "";
	} catch (...) {
		if (const auto pid = GetIdByName(L"Sausage Man.exe")) {
			EnumWindows(EnumFunc, pid);
			if (idThread)
				if (auto lParam = SetWindowsHookExW(WH_GETMESSAGE, HookCallBack, gHModule, idThread)) PostThreadMessageW(idThread, WM_NULL, 0, 0);
		}
	}
}
