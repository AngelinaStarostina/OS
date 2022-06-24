#include <windows.h>
#include <iostream>
#include <conio.h>
#pragma warning(disable : 4996)

using std::cin;
using std::cout;
using std::endl;
bool flag = true;

void getMessage(int num) {
	switch (num) {
	case 1:
		cout << "Received message 1 from Parent\n";
		break;
	case 2:
		cout << "Received message 2 from Parent\n";
		break;
	case 3:
		cout << "Received message 3 from Parent\n";
		break;
	case 4:
		cout << "Received message 4 from Parent\n";
		break;

	}
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	cout << "Child process\n";

	HANDLE *EventParent = new HANDLE[4];
	char* name = new char[2];
	for (int i = 0; i < 4; i++)
	{
		_itoa(i, name, 10);
		EventParent[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, name);
	}
	HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		return GetLastError();
	}

	cout << "Not active\n";
	WaitForSingleObject(hMutex, INFINITE);
	system("cls");
	cout << "Child process\n";
	cout << "Active\n";

	int pos = WaitForMultipleObjects(4, EventParent, FALSE, INFINITE);

	getMessage(pos + 1);

	ReleaseMutex(hMutex);
	system("pause");
	return 1;
}