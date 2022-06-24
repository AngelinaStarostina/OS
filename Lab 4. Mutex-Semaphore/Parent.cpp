#include <windows.h>
#include <iostream>
#include <conio.h>
#pragma warning(disable : 4996)
using std::cin;
using std::cout;
using std::endl;
volatile bool flag = true;


void getMessage(int num) {
	switch (num) {
	case 1:
		cout << "Received message 1 from Boss\n";
		break;
	case 2:
		cout << "Received message 2 from Boss\n";
		break;
	case 3:
		cout << "Received message 3 from Boss\n";
		break;
	case 4:
		cout << "Received message 4 from Boss\n";
		break;

	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	cout << "Parent process\n";

	HANDLE *handleArray = new HANDLE[4];
	handleArray[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event1");
	handleArray[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event2");
	handleArray[2] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event3");
	handleArray[3] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event4");
	HANDLE *EventParent = new HANDLE[4];
	char* name = new char[2];
	for (int i = 0; i < 4; i++)
	{
		_itoa(i, name, 10);
		EventParent[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, name);
	}
	HANDLE semaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, "Semaphore");

	cout << "Not active\n";
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	cout << "Parent process\n";
	cout << "Active\n";

	int pos = WaitForMultipleObjects(4, handleArray, FALSE, INFINITE);

	getMessage(pos + 1);

	if (pos + 1 == 4)
	{
		SetEvent(EventParent[0]);
	}
	else
	{
		SetEvent(EventParent[pos + 1]);
	}

	ReleaseSemaphore(semaphore, 1, NULL);

	system("pause");
	return 1;
}