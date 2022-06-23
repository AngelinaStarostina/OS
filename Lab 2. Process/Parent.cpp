#include <windows.h>
#include <conio.h>
#include<iostream>
using std::cin;
using std::cout;
#pragma warning(disable : 4996)

int main(int argc, char* argv[])
{
	char* name = (char*)"Child.exe";
	int n = strlen(name);
	for (int i = 1; i < argc - 1; i++)
		n += strlen(argv[i]);
	char* lpszCommandLine = new char[n];
	strcpy(lpszCommandLine, name);
	for (int i = 1; i < argc - 1; i++)
	{
		strcat(lpszCommandLine, " ");
		strcat(lpszCommandLine, (char*)argv[i]);
	}

	
	PROCESS_INFORMATION piCom;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwXCountChars = 1000;		

	PROCESS_INFORMATION piCom2;
	STARTUPINFO si2;
	ZeroMemory(&si2, sizeof(STARTUPINFO));
	si2.cb = sizeof(STARTUPINFO);

	CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);

	while (true)
	{
		char c;
		_cputs("Input 't' to terminate the process: ");
		c = _getch();
		if (c == 't')
		{
			_cputs("t\n");
			TerminateProcess(piCom.hProcess, 1);
			break;
		}
	}

	char lpszCommandLine2[] = "Count.exe";

	CreateProcess(NULL, lpszCommandLine2, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si2, &piCom2);

	WaitForSingleObject(&piCom2, INFINITE);

	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);
	CloseHandle(piCom2.hThread);
	CloseHandle(piCom2.hProcess);

	_cputs("The new process is created.\n");
	_cputs("Press any key to finish.\n");
	_getch();
	ExitProcess(0);
}
