/**
 * @Description: a class for convert SEH exception to c++ exception
 * @Author: shang_cm
 * @Date: 2020-06-01 16:09:00
 * @LastEditTime: 2010-06-01 16:09:00
 * @LastEditors: shang_cm
 */
#pragma once
#include <windows.h>
#include <dbghelp.h>
#include <exception>
#include <Encode.h>

#define STACK_INFO_LEN 2048

class SEH_exception final : std::exception {
	using fpRtlNtStatusToDosError = ULONG(WINAPI*)(NTSTATUS Status);

public:
	SEH_exception(const unsigned int nExceptionCode, _EXCEPTION_POINTERS* pstExcptionInfor) : m_hNtModule(nullptr), m_nExceptionCode(0), m_szMsgBuff(nullptr), m_pExcptionInfor(nullptr), RtlNtStatusToDosError(nullptr) {
		m_nExceptionCode = nExceptionCode;
		m_pExcptionInfor = pstExcptionInfor;
		m_hNtModule      = GetModuleHandleW(L"NTDLL.DLL");
		if (nullptr != m_hNtModule) RtlNtStatusToDosError = reinterpret_cast<fpRtlNtStatusToDosError>(GetProcAddress(m_hNtModule, "RtlNtStatusToDosError"));
	}

	~SEH_exception() override {
		m_nExceptionCode      = 0;
		m_pExcptionInfor      = nullptr;
		RtlNtStatusToDosError = nullptr;

		if (nullptr != m_szMsgBuff) {
			LocalFree(m_szMsgBuff);
			m_szMsgBuff = nullptr;
		}
	};

	[[nodiscard]] auto what() const noexcept -> const char* override {
		if (RtlNtStatusToDosError != nullptr) {
			if (const DWORD nConvertLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE, m_hNtModule, RtlNtStatusToDosError(m_nExceptionCode), 0, (char*)&m_szMsgBuff, 0, nullptr); 0 != nConvertLen) 
				return encode::GbkToUtf8(m_szMsgBuff).data();
		}
		return "SEH_UNKNOW_ERROR";
	}

	[[nodiscard]] auto info() const -> PEXCEPTION_POINTERS { return m_pExcptionInfor; }

	[[nodiscard]] auto code() const -> unsigned int { return m_nExceptionCode; }

private:
	HMODULE m_hNtModule;

	unsigned int            m_nExceptionCode;
	char*                   m_szMsgBuff;
	PEXCEPTION_POINTERS     m_pExcptionInfor;
	fpRtlNtStatusToDosError RtlNtStatusToDosError;

public:
	static auto (__cdecl TranslateSEHtoCE)(const unsigned int nExceptionCode, struct _EXCEPTION_POINTERS* pstExcptionInfor) -> void { throw SEH_exception(nExceptionCode, pstExcptionInfor); }
};

class SEH {
public:
    static void ShowTraceStack(const char* szBriefInfo) {
        static constexpr int MAX_STACK_FRAMES = 32;
        void* pStack[MAX_STACK_FRAMES];
        static char szStackInfo[STACK_INFO_LEN * MAX_STACK_FRAMES];
        static char szFrameInfo[STACK_INFO_LEN];

        const HANDLE process = GetCurrentProcess();
        SymInitialize(process, nullptr, TRUE);
        const WORD frames = CaptureStackBackTrace(0, MAX_STACK_FRAMES, pStack, nullptr);
        strcpy(szStackInfo, szBriefInfo == nullptr ? "stack traceback:\n" : szBriefInfo);

        for (WORD i = 0; i < frames; ++i) {
            const DWORD64 address = reinterpret_cast<DWORD64>(pStack[i]);

            DWORD64    displacementSym = 0;
            char       buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            const auto pSymbol    = reinterpret_cast<PSYMBOL_INFO>(buffer);
            pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymbol->MaxNameLen   = MAX_SYM_NAME;

            DWORD displacementLine = 0;
            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            if (SymFromAddr(process, address, &displacementSym, pSymbol) &&
                SymGetLineFromAddr64(process, address, &displacementLine, &line))
            {
                snprintf(szFrameInfo, sizeof(szFrameInfo), "\t%s() at %s:%d(0x%x)\n",
                    pSymbol->Name, line.FileName, line.LineNumber, pSymbol->Address);
            }
            else
            {
                snprintf(szFrameInfo, sizeof(szFrameInfo), "\t %p\n", address);
            }
            strcat(szStackInfo, szFrameInfo);
        }

        std::cout << szStackInfo; // 输出到控制台，也可以打印到日志文件中
    }
};