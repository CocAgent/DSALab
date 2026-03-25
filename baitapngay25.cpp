#include <iostream>
#include <algorithm>
using namespace std;

void selectionsort(int a[], int n)
{
    int vt;
    for (int i = 0; i < n - 1; i++) {
        vt = i;
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[vt])
                vt = j;
        swap(a[i], a[vt]);
    }
}

void Shift(int a[], int l, int r) {
    int i = l;
    int j = 2 * i + 1;
    int x = a[i];

    while (j <= r) {
        if (j < r && a[j] < a[j + 1]) j++;
        if (x >= a[j]) break;

        a[i] = a[j];
        i = j;
        j = 2 * i + 1;
    }

    a[i] = x;
}

void HeapSort(int a[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        Shift(a, i, n - 1);

    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        Shift(a, 0, i - 1);
    }
}

void insertionSort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int x = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = x;
    }
}

void BubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

void QuickSort(int a[], int l, int r) {
    int i = l, j = r;
    int x = a[(l + r) / 2];

    while (i <= j) {
        while (a[i] < x) i++;
        while (a[j] > x) j--;
        if (i <= j) {
            swap(a[i], a[j]);
            i++; j--;
        }
    }

    if (l < j) QuickSort(a, l, j);
    if (i < r) QuickSort(a, i, r);
}

int main() {
    int n;
    cout << "Nhap so phan tu: ";
    cin >> n;

    int a[100];

    cout << "Nhap vao mang:\n";
    for (int i = 0; i < n; i++)
    {
        cout << "a[" << i << "] = ";
        cin >> a[i];
    }

    // Chọn thuật toán ở đây
    selectionsort(a, n);
    // HeapSort(a, n);
    // insertionSort(a, n);
    // BubbleSort(a, n);
    // QuickSort(a, 0, n - 1);

    cout << "Mang sau khi sap xep: ";
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";

    return 0;
}