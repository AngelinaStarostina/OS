#include <iostream>
#include <windows.h>
#include <process.h>
using std::cin;
using std::cout;
using std::endl;

HANDLE event1, event2;
CRITICAL_SECTION critSection;
char* sortedArr = NULL;
int numElements;
int countElements = 0;

void outArray(int time)
{
	for (int i = 0; i < numElements; i++)
	{
		cout << sortedArr[i] << endl;
		Sleep(time);
	}
}

void WINAPI CountElement(char* arr)
{
	EnterCriticalSection(&critSection);
	WaitForSingleObject(event2, INFINITE);
	for (int i = 0; i < numElements; i++)
	{
		if (!isspace(sortedArr[i]))
			countElements++;
	}

	cout << "CountElement: " << endl;
	outArray(0);

	LeaveCriticalSection(&critSection);
}

UINT WINAPI Work(char* arr)
{
	int time;
	cout << "Enter the time interval required to rest after preparing one element in the array: ";
	cin >> time;
	sortedArr = new char[numElements];

	int j = 0;
	char H;
	for (int i = 0; i < numElements; i++)
	{
		H = (char)arr[i];
		if (H < 48)
		{
			sortedArr[j] = H;
			j++;
		}
		else if (H > 57 && H < 65)
		{
			sortedArr[j] = H;
			j++;
		}
		else if (H > 90 && H < 97)
		{
			sortedArr[j] = H;
			j++;
		}
		else if (H > 122)
		{
			sortedArr[j] = H;
			j++;
		}
	}

	if (j != numElements)
	{
		for (int i = j; i < numElements; i++)
			sortedArr[i] = ' ';
	}

	outArray(time);

	cout << endl;
	SetEvent(event1);
	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");

	event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	event2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	InitializeCriticalSection(&critSection);

	char* arr = NULL;
	cout << "Enter the number of array elements: ";
	cin >> numElements;
	cout << "Enter array elements: ";
	arr = new char[numElements + 1];
	for (int i = 0; i < numElements; i++)
		cin >> arr[i];

	HANDLE worker;
	worker = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Work, (void*)arr, 0, NULL);
	if (worker == 0)
		return GetLastError();

	HANDLE countEl;
	countEl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CountElement, (void*)arr, 0, NULL);
	if (countEl == 0)
		return GetLastError();


	WaitForSingleObject(event1, INFINITE);
	cout << "Final array: " << endl;
	outArray(0);

	SetEvent(event2);

	EnterCriticalSection(&critSection);
	cout << "The number of characters corresponding to the punctuation marks of the final array: " << countElements << endl;
	cout << "Final array (CriticalSection): " << endl;
	outArray(0);
	LeaveCriticalSection(&critSection);

	WaitForSingleObject(countEl, INFINITE);
	WaitForSingleObject(worker, INFINITE);

	CloseHandle(countEl);
	CloseHandle(worker);
	delete[] arr;
	DeleteCriticalSection(&critSection);
	CloseHandle(event2);
	CloseHandle(event1);
	delete[] sortedArr;

	return 0;
}