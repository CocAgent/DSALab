#include <iostream>
#include <algorithm> // Thư viện chứa hàm swap
using namespace std;

// --- TIỆN ÍCH ---
void Nhap(int a[], int& n) {
    cout << "Nhap n: "; cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "a[" << i << "] = "; cin >> a[i];
    }
}

void Xuat(int a[], int n, string s) {
    cout << "=> " << s << ": [ ";
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << "]\n";
}

// --- TÌM KIẾM ---
// Xấu nhất: x ở cuối hoặc không có O(n)
int TimTuyenTinh(int a[], int n, int x) {
    for (int i = 0; i < n; i++) if (a[i] == x) return i;
    return -1;
}

// Xấu nhất: x nằm ở các nhánh cuối của cây nhị phân O(log n)
int TimNhiPhan(int a[], int n, int x) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == x) return m;
        (a[m] < x) ? l = m + 1 : r = m - 1;
    }
    return -1;
}

// --- SẮP XẾP ---
// Hiệu suất luôn là O(n^2) vì luôn quét tìm min
void SelectionSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int m = i;
        for (int j = i + 1; j < n; j++) if (a[j] < a[m]) m = j;
        swap(a[i], a[m]);
    }
}

// Tốt nhất: O(n) khi mảng đã xếp xong
void InsertionSort(int a[], int n) {
    for (int i = 1, j, x; i < n; i++) {
        x = a[i]; j = i - 1;
        while (j >= 0 && a[j] > x) { a[j + 1] = a[j]; j--; }
        a[j + 1] = x;
    }
}

// Tốt nhất: O(n log n) | Xấu nhất: O(n^2) khi pivot chọn sai
void QuickSort(int a[], int l, int r) {
    int i = l, j = r, x = a[(l + r) / 2];
    while (i <= j) {
        while (a[i] < x) i++;
        while (a[j] > x) j--;
        if (i <= j) swap(a[i++], a[j--]);
    }
    if (l < j) QuickSort(a, l, j);
    if (i < r) QuickSort(a, i, r);
}

int main() {
    int a[100], n, x;

    Nhap(a, n);
    Xuat(a, n, "Mang vua nhap");

    cout << "\nNhap gia tri can tim: "; cin >> x;
    int vt = TimTuyenTinh(a, n, x);
    (vt != -1) ? cout << "Tim thay tai vi tri: " << vt : cout << "Khong tim thay!";

    cout << "\n\n--- THUC HIEN SAP XEP ---";
    QuickSort(a, 0, n - 1); // Chọn giải thuật nhanh nhất làm ví dụ
    Xuat(a, n, "Mang sau khi QuickSort");

    cout << "\nNhap gia tri can tim (Nhi phan): "; cin >> x;
    vt = TimNhiPhan(a, n, x);
    (vt != -1) ? cout << "Tim thay tai: " << vt : cout << "Khong thay!";

    return 0;
}
