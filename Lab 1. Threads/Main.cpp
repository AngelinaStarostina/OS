#include <windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
using std ::cin;
using std::cout;

const int maxNumArr = 100;

UINT WINAPI workerThread(LPVOID *arr)
{
	int count = 0;
	for (int i = 1; i < (int)arr[0] + 1; i++)
	{
		if ((int)arr[i] % 2 == 0 && (int)arr[i] != 0)
			count++;
	}

	cout << "Number of even elements: " << count;
	return 0;
}

int mainThread()
{
	int length;
	int timeThread;
	cout << "Enter the number of array elements: ";
	cin >> length;
	int* arr = new int[length + 1];
	arr[0] = length;
	cout << "Enter the time to stop and start the thread: ";
	cin >> timeThread;

	srand(time(0));
	for (int i = 1; i < length + 1; i++)		
	{
		arr[i] = rand() % maxNumArr;
		cout << arr[i] << "\n";
	}

	HANDLE worker;
	UINT IDThreadWorker;
	worker = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)workerThread, (void*)arr, 0, &IDThreadWorker);
	if (worker == 0)
		return GetLastError();

	SuspendThread(worker);
	Sleep(timeThread);									
	ResumeThread(worker);

	WaitForSingleObject(worker, INFINITE);

	CloseHandle(worker);

	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");
	HANDLE main;
	DWORD IDThreadMain;

	main = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mainThread, 0, 0, &IDThreadMain);
	if (main == NULL)
		return GetLastError();

	WaitForSingleObject(main, INFINITE);

	CloseHandle(main);

	return 0;
}