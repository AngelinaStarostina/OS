#include<iostream>
#include <conio.h>
using std::cin;
using std::cout;
using std::endl;

int main()
{
	int a = 1, b = 1, c = 1;
	int n, count = 0;
	cout << "Enter the number of elements: ";
	cin >> n;
	while (count < n) 
	{        
		cout << "c = " << c << endl;
		c = a + b;
		a = b;
		b = c;
		count++;
	}

	system("pause");
	return 0;
}
