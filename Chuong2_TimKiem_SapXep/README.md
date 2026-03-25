# 📌 Pointer (Con trỏ) trong C++

## 🧠 Định nghĩa

**Con trỏ (pointer)** là biến dùng để lưu **địa chỉ của một biến khác trong bộ nhớ**.

* Biến thường → lưu giá trị
* Con trỏ → lưu địa chỉ

---

## 🔑 Cú pháp cơ bản

```cpp
int a = 10;
int *p = &a;
```

* `a`: biến thường
* `&a`: địa chỉ của `a`
* `p`: con trỏ lưu địa chỉ
* `*p`: giá trị tại địa chỉ đó

---

## ⚙️ Tính chất của con trỏ

### 🔹 Truy cập gián tiếp

```cpp
cout << *p;
```

### 🔹 Thay đổi giá trị qua con trỏ

```cpp
*p = 20;
```

### 🔹 Cùng kiểu dữ liệu

```cpp
int *p;
float *q;
```

### 🔹 Con trỏ NULL

```cpp
int *p = NULL;
```

### 🔹 Phép toán con trỏ

```cpp
p++;
```

---

## 📌 Ví dụ hoàn chỉnh

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 5;
    int *p = &a;

    cout << "Gia tri a: " << a << endl;
    cout << "Dia chi a: " << &a << endl;
    cout << "Con tro p: " << p << endl;
    cout << "Gia tri tai p: " << *p << endl;

    *p = 100;
    cout << "Sau khi doi, a = " << a << endl;

    return 0;
}
```

---

## 🧩 Con trỏ với mảng

```cpp
int arr[] = {1, 2, 3, 4};
int *p = arr;

cout << *(p + 2); // 3
```

---

## 🧪 Bài tập thực hành

### 🔸 Hoán đổi 2 số

```cpp
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
```

```cpp
int x = 5, y = 10;
swap(&x, &y);
```

---

### 🔸 Tính tổng mảng

```cpp
int sum(int *arr, int n) {
    int s = 0;
    for(int i = 0; i < n; i++) {
        s += *(arr + i);
    }
    return s;
}
```

---

### 🔸 Tìm số lớn nhất

```cpp
int max(int *arr, int n) {
    int m = arr[0];
    for(int i = 1; i < n; i++) {
        if(*(arr + i) > m)
            m = *(arr + i);
    }
    return m;
}
```

---

### 🔸 Cấp phát động

```cpp
int *p = new int;
*p = 50;

delete p;
```

---

## 🚀 Tóm tắt nhanh

| Ký hiệu | Ý nghĩa                 |
| ------- | ----------------------- |
| `&`     | Lấy địa chỉ             |
| `*`     | Lấy giá trị tại địa chỉ |

👉 Con trỏ giúp:

* Truyền tham chiếu
* Làm việc với mảng
* Quản lý bộ nhớ

---

## ✨ Mở rộng

* Con trỏ 2 cấp (`int **p`)
* Con trỏ với struct/class
* Dynamic array
* Smart pointer (C++ hiện đại)

---

## Xuất ra

Gia tri a: 10
Dia chi a: 000000C3A42FF6C0
Con tro p: 000000C3A42FF6C0
Gia tri tai p (*p): 10
Sau khi doi, a = 50
------------------------
Truoc swap: x = 5, y = 20
Sau swap: x = 20, y = 5
------------------------
Tong mang: 15
Max mang: 5
Phan tu thu 3 (pointer): 3
------------------------
Gia tri cap phat dong: 100
