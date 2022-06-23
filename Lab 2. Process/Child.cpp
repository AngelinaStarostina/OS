#include <conio.h>
#include<iostream>
using std::cin;
using std::cout;
int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		int n = atoi(argv[1]);
		int* arr = new int[n];
		for (int i = 1; i < argc; i++)
			arr[i - 1] = atoi(argv[i]);

		int counter = 0;
		for (int i = 1; i < n; i++) {
			for (int j = i; j > 0 && arr[j - 1] > arr[j]; j--) {
				int tmp = arr[j - 1];
				arr[j - 1] = arr[j];
				arr[j] = tmp;
			}
		}

		for (int i = 0; i < n; i++)
			cout << arr[i] << " ";

		delete[] arr;
	}
	_getch();
	return 0;
}
