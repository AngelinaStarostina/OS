#include <windows.h>
#include <iostream>
#include <time.h>
using std::cin;
using std::cout;
using std::endl;

int main() {
	HANDLE hNamedPipe;
	char machineName[80];
	char pipeName[80];
	DWORD dwBytesWritten;
	DWORD dwBytesRead; 

	cout << "Enter a name of the server machine (. - default): ";
	cin >> machineName;
	wsprintf(pipeName, "\\\\%s\\pipe\\demo_pipe", machineName);


	hNamedPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Invalid handle value.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	int numArrElements;

	if (!ReadFile(hNamedPipe, &numArrElements, sizeof(numArrElements), &dwBytesRead, (LPOVERLAPPED)NULL)) {
		cout << "Read size failed\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}
	cout << "The number " << numArrElements << " is read from the pipe" << endl;

	char* arr = new char[numArrElements];
	for (int i = 0; i < numArrElements; i++)
	{
		
		if (!ReadFile(hNamedPipe, &arr[i], sizeof(arr[i]), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			cout << "Read number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return GetLastError();
		}

		cout << "The number " << arr[i] << " is read from the pipe" << endl;
	}
	cout << endl;

	int count;
	do {
		cout << "Enter the number of elements:  ";
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
			cout << "The element has already been consumed" << endl;
		}
		else
		{
			cout << "Element " << arr[i] << " consumed" << endl;
			arr[i] = NULL;
		}
		count--;

	}

	int n2 = numArrElements - count2;

	if (!WriteFile(hNamedPipe, &n2, sizeof(n2), &dwBytesWritten, (LPOVERLAPPED)NULL))
	{
		cout << "Write size failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}
	cout << n2 << " is written to the pipe.\n";

	for (int i = 0; i < numArrElements; i++) {
		if (arr[i] != NULL) 
		{
			if (!WriteFile(hNamedPipe, &arr[i], sizeof(arr[i]), &dwBytesWritten, (LPOVERLAPPED)NULL)) 
			{
				cout << "Write number failed.\n";
				system("pause");
				return GetLastError();
			}
			cout << arr[i] << " is written to the pipe.\n";
		}
	}

	delete[] arr;
	CloseHandle(hNamedPipe);

	system("pause");
	return 0;
}