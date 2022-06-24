#include <windows.h>
#include <iostream>
#include <conio.h>
#pragma warning(disable : 4996)
using std::cin;
using std::cout;
using std::endl;

HANDLE event1;
HANDLE event2;
HANDLE event3;
HANDLE event4;
HANDLE *EventParent = new HANDLE[4];
HANDLE semaphore;
HANDLE hMutex;

struct ProcessInformation {
	STARTUPINFO info;
	PROCESS_INFORMATION processInformation;

	ProcessInformation() {
		ZeroMemory(&info, sizeof(STARTUPINFO));
		info.cb = sizeof(STARTUPINFO);
	}
};

void sendMessage(int type) {
	switch (type) {
	case 1:
		SetEvent(event1);
		break;
	case 2:
		SetEvent(event2);
		break;
	case 3:
		SetEvent(event3);
		break;
	case 4:
		SetEvent(event4);
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	event1 = CreateEvent(NULL, FALSE, FALSE, "Event1");
	event2 = CreateEvent(NULL, FALSE, FALSE, "Event2");
	event3 = CreateEvent(NULL, FALSE, FALSE, "Event3");
	event4 = CreateEvent(NULL, FALSE, FALSE, "Event4");
	char* name = new char[2];
	for (int i = 0; i < 4; i++)
	{
		_itoa(i, name, 10);
		EventParent[i] = CreateEvent(NULL, FALSE, FALSE, name);
	}
	semaphore = CreateSemaphore(NULL, 2, 2, "Semaphore");
	hMutex = CreateMutex(NULL, FALSE, "Mutex");

	int numParent, numChild;
	cout << "Enter the number of Parent processes : ";
	cin >> numParent;
	cout << "Enter the number of Child processes: ";
	cin >> numChild;

	ProcessInformation **processInformation;
	processInformation = new ProcessInformation*[numChild + numParent];

	char parentCommandLine[] = "Parent.exe";
	char childCommandLine[] = "Child.exe";
	for (int i = 0; i < numChild; i++) {
		processInformation[i] = new ProcessInformation();
		if (!CreateProcess(NULL, childCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i]->info, &processInformation[i]->processInformation)) {
			cout << "Child not created\n";
		}
	}
	for (int i = 0; i < numParent; i++) {
		processInformation[i + 1] = new ProcessInformation();
		if (!CreateProcess(NULL, parentCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i + 1]->info, &processInformation[i + 1]->processInformation)) {
			cout << "Parent not created\n";
		}
	}
	
	int numMessParent, numMessChild;
	cout << "Enter the number of messages sent to Parent: ";
	cin >> numMessParent;

	int message;
	for (int i = 0; i < numMessParent; i++) {
		
		if(i != 0)
			cout << "Message " << message << " sent" << endl;

		cout << "Enter a message (a number from 1 to 4):\n";
		cin >> message;
		if (message < 1 || message > 4)
			i--;
		
		sendMessage(message);
	}

	delete[] name;
	CloseHandle(hMutex);
	CloseHandle(semaphore);
	CloseHandle(event4);
	CloseHandle(event3);
	CloseHandle(event2);
	CloseHandle(event1);

	system("pause");

	return 0;
}