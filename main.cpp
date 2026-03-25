#include "header.h"

int menu()
{
	system("cls");

	int cv;
	cout << "=== MENU ===";
	cout << "\n1. SelectionSort";
	cout << "\n2. InsertSort";
	cout << "\n3. BubbleSort";
	cout << "\n4. QuickSort";
	cout << "\n5. HeapSort";
	do
	{
		cout << "\nNhap thao tac: ";
		cin >> cv;
	} while (cv<0);
	return cv;
}


int main()
{
	int a[] = { 3,-12,14,9,11,-50,-100 };
	int n = 7;
	int cv;
	do
	{
		cv = menu();
		switch (cv)
		{
		case 1:
			SelectionSort(a,n);
			cout << "ket qua sau khi SelectionSort: ";
			XuatMang(a, n);
			break;
		case 2:
			InsertionSort(a,n);
			cout << "ket qua sau khi InsertionSort: ";
			XuatMang(a, n);
			break;
		case 3:
			BubbleSort(a,n);
			cout << "ket qua sau khi BubbleSort: ";
			XuatMang(a, n);
			break;
		case 4:
			QuickSort(a,0,n-1);
			cout << "ket qua sau khi QuickSort: ";
			XuatMang(a,n);
			break;
		case 5:
			Heapsort(a,n);
			cout << "ket qua sau khi HeapSort: ";
			XuatMang(a, n);
			break;
		}
		cout << endl;
		system("pause");
	} while (cv);
	return 0;
}