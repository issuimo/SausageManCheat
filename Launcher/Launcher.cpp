#include <Windows.h>

int main() {
    system("chcp 65001");
    HMODULE hmodule = LoadLibrary(L"GPP32.dll");
    FARPROC inject = GetProcAddress(hmodule, "WindowsMessageInject");
    inject();
}
