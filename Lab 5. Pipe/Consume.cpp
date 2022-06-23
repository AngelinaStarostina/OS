#include <windows.h>
#include <iostream>
#include <time.h>
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	HANDLE hReadPipe;
	HANDLE hWritePipe;
	HANDLE hEnableRead;
	char lpszEnableRead[] = "EnableRead";
	HANDLE hEnableRead2;
	char lpszEnableRead2[] = "EnableRead2";

	hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);
	hEnableRead2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead2);

	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);

	WaitForSingleObject(hEnableRead2, INFINITE);

	DWORD dwBytesRead;
	int numArrElements;
	if (!ReadFile(hReadPipe, &numArrElements, sizeof(numArrElements), &dwBytesRead, NULL))
	{
		cout << "Read from the pipe failed.\n";
		return GetLastError();
	}
	cout << "The number " << numArrElements << " is read from the pipe" << endl;
	char* arr = new char[numArrElements];
	for (int i = 0; i < numArrElements; i++)
	{
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			return GetLastError();
		}
		cout << "The number " << arr[i] << " is read from the pipe" << endl;
	}
	cout << endl;


	int count;
	do {
		cout << "Enter the number of numbers to be consumed:  ";
		cin >> count;
	} while (numArrElements < count);


	int count2 = count;

	srand(time(NULL));
	while (count != NULL)
	{
		int i = rand() % (numArrElements - 1);
		if (arr[i] == NULL)
		{
			count2--;
		}		
		cout << "Element " << arr[i] << " consumed" << endl;
		arr[i] = NULL;
		count--;
	}

	int n2 = numArrElements - count2;

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n2, sizeof(n2), &dwBytesWritten, NULL))
	{
		cout << "Write n to file failed.\n";
		return GetLastError();
	}
	cout << n2 << "is written to the pipe.\n";

	for (int j = 0; j < numArrElements; j++)
	{
		DWORD dwBytesWritten;
		if (arr[j] != NULL)
		{
			if (!WriteFile(hWritePipe, &arr[j], sizeof(arr[j]), &dwBytesWritten, NULL))
			{
				cout << "Write to file failed.\n";
				return GetLastError();
			}
			cout << arr[j] << " is written to the pipe.\n";
		}
	}

	SetEvent(hEnableRead);

	
	CloseHandle(hEnableRead2);
	CloseHandle(hEnableRead);
	delete[] arr;
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	system("pause");
	return 0;
}