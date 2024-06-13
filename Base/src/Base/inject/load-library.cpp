#include <pch.h>
#include "load-library.h"
#include <Base/util.h>

#if defined(DISABLE_OUTPUT)
#define ILog(data, ...)
#define IPrintError(text, ...)
#else
#define ILog(text, ...) printf(text, __VA_ARGS__)
#define ILogError(text, ...) ILog(text, __VA_ARGS__); std::cout << "错误: " << util::GetLastErrorAsString() << std::endl
#endif

bool LoadLibraryDLL(HANDLE hProc, const std::string& dllpath)
{
	HMODULE hKernel = GetModuleHandle("kernel32.dll");
	if (hKernel == NULL)
	{
		ILogError("[DLL注入]获取kernel32.dll模块地址失败.\n");
		return false;
	}

	LPVOID pLoadLibrary = (LPVOID)GetProcAddress(hKernel, "LoadLibraryA");
	if (pLoadLibrary == NULL) {
		ILogError("[DLL注入]获取LoadLibraryA地址失败.\n");
		return false;
	}

	LPVOID pDLLPath = VirtualAllocEx(hProc, NULL, strlen(dllpath.c_str()) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (pDLLPath == NULL) {
		ILogError("在目标进程中为DLLPath分配内存失败.\n");
		return false;
	}

	// Write the string name of our DLL in the memory allocated
	BOOL writeResult = WriteProcessMemory(hProc, pDLLPath, dllpath.c_str(), strlen(dllpath.c_str()), NULL);
	if (writeResult == FALSE) {
		ILogError("[DLL注入]写远程进程内存失败.\n");
		return false;
	}

	// Load our DLL by calling loadlibrary in the other process and passing our dll name
	HANDLE hThread = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibrary, (LPVOID)pDLLPath, NULL, NULL);
	if (hThread == NULL) {
		ILogError("[DLL注入]创建远程线程失败.\n");
		VirtualFreeEx(hProc, pDLLPath, 0, MEM_RELEASE);
		return false;
	}

	// Waiting for thread end and release unnecessary data.
	if (WaitForSingleObject(hThread, 2000) == WAIT_OBJECT_0) 
	{
		ILog("[DLL注入]远程线程成功结束.\n");
		VirtualFreeEx(hProc, pDLLPath, 0, MEM_RELEASE);
	}

	CloseHandle(hThread);

	ILog("[DLL注入]LoadLibraryA注入成功.\n");
	return true;
}