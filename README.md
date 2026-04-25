# 📚 Hệ Thống Quản Lý Thư Viện
> C++17 · 4 Cấu trúc dữ liệu · 6 Test cases · Menu console đầy đủ

---

## 1. Bài toán

Xây dựng phần mềm quản lý thư viện chạy trên terminal, giải quyết 4 vấn đề thực tế:

| Vấn đề thực tế | Giải pháp trong hệ thống |
|----------------|--------------------------|
| Kho sách lớn, thêm/xóa thường xuyên | Doubly Linked List — thêm/xóa O(1) |
| Tra cứu sách/độc giả phải nhanh | Hash Table — tìm kiếm O(1) trung bình |
| Sách hết bản, nhiều người muốn mượn | Queue — xếp hàng công bằng FIFO |
| Thao tác nhầm cần hoàn tác | Stack — lưu lịch sử, undo như Ctrl+Z |

---

## 2. Thiết kế dữ liệu

### Các struct cốt lõi

```cpp
struct Book {
    string isbn;             // Mã định danh duy nhất
    string title;            // Tên sách
    string author;           // Tác giả
    string genre;            // Thể loại
    int    totalCopies;      // Tổng số bản trong thư viện
    int    availableCopies;  // Số bản còn có thể mượn
};

struct Reader {
    string id;                       // Mã độc giả
    string name;                     // Họ tên
    string phone;                    // Số điện thoại
    vector<string> borrowedISBNs;    // Danh sách ISBN đang mượn
};

struct Transaction {
    string action;       // "BORROW" | "RETURN" | "ADD_BOOK" | "DELETE_BOOK"
    string readerId;     // Ai thực hiện
    string isbn;         // Sách nào
    string borrowDate;   // Ngày thực hiện
    string dueDate;      // Hạn trả (nếu là BORROW)
    string returnDate;   // Ngày trả thực tế (nếu là RETURN)
};
```

### Bộ 4 CTDL và lý do chọn

#### CTDL 1 — Doubly Linked List: lưu kho sách

Kho sách cần thêm và xóa liên tục. Nếu dùng mảng, mỗi lần xóa phải dịch chuyển toàn bộ phần tử phía sau — O(n) mỗi lần xóa, rất chậm khi kho lớn. Danh sách liên kết đôi cho phép thêm/xóa O(1) khi đã có con trỏ, đồng thời duyệt được cả hai chiều để hiển thị danh sách.

```
         prev          prev          prev
NULL ←──────── [Dế Mèn] ⟺ [Số Đỏ] ⟺ [Hoàng Tử Bé] ⟺ [Tắt Đèn] ──────── NULL
  ↑ head                                                    ↑ tail       next
```

#### CTDL 2 — Hash Table: tra cứu sách và độc giả

Tra cứu theo ISBN và mã độc giả là thao tác xảy ra nhiều nhất. Hash Table ánh xạ key thành chỉ số bucket qua hàm hash, cho phép tìm kiếm O(1) trung bình — không cần duyệt toàn bộ danh sách. Dùng chaining (vector trong mỗi bucket) để xử lý đụng độ.

```
"ISBN001" → h("ISBN001") % 101 = 42 → bucket[42] → {Dế Mèn, còn 2/3 bản}
"ISBN002" → h("ISBN002") % 101 = 17 → bucket[17] → {Số Đỏ, còn 0/1 bản}
"R001"    → h("R001")    % 101 = 63 → bucket[63] → {Nguyễn Văn A, mượn 1 cuốn}
```

#### CTDL 3 — Queue: hàng chờ khi sách hết bản

Khi sách hết bản (`availableCopies == 0`), độc giả được xếp vào hàng chờ. Queue hoạt động theo FIFO — ai đặt trước được phục vụ trước, đúng nguyên tắc công bằng. Khi có người trả sách, hệ thống tự động lấy người đầu hàng ra và cấp sách.

```
Sách "Số Đỏ" (ISBN002) hết bản:

  enqueue(R002)     enqueue(R003)
       ↓                 ↓
  [ R002 → R003 → R004 ]
    ↑ front              ↑ back

  Khi R001 trả sách:
  dequeue() → R002 được mượn tự động
  [ R003 → R004 ]
```

#### CTDL 4 — Stack: lịch sử giao dịch và undo

Mỗi thao tác (mượn, trả, thêm sách...) được push vào Stack. Khi cần hoàn tác, pop phần tử trên cùng và đảo ngược thao tác tương ứng. Stack LIFO khớp đúng với hành vi "Ctrl+Z" — thao tác gần nhất luôn được hoàn tác trước.

```
        push              push              push
          ↓                 ↓                 ↓
   ┌──────────────────────────────────────┐
   │  [TOP] BORROW | R003 | ISBN003       │ ← pop để undo
   │        BORROW | R001 | ISBN002       │
   │        ADD_BOOK | SYSTEM | ISBN001   │
   └──────────────────────────────────────┘
```

---

## 3. Luồng xử lý chính

```
Độc giả yêu cầu mượn sách
            │
            ▼
   Độc giả có trong hệ thống? ── Không ──► Báo lỗi, dừng
            │ Có
            ▼
   Sách tồn tại trong kho? ────── Không ──► Báo lỗi, dừng
            │ Có
            ▼
   availableCopies > 0? ─────── Không ──► enqueue(readerId, isbn)
            │ Có                                    │
            ▼                                       ▼
   availableCopies--                    Thông báo vị trí hàng chờ
   borrowedISBNs.push(isbn)
            │
            ▼
   push Transaction vào Stack lịch sử
            │
            ▼
   Hiển thị hạn trả (ngày hôm nay + 14 ngày)


─────────────────────────────────────────────────

Độc giả trả sách
            │
            ▼
   Xóa isbn khỏi borrowedISBNs của độc giả
            │
            ▼
   availableCopies++
            │
            ▼
   Queue hàng chờ có người? ── Có ──► Thông báo người đầu hàng
            │ Không                   đến lấy sách
            ▼
   push Transaction vào Stack lịch sử
```

---

## 4. Cấu trúc file và lý do tổ chức

```
MSSV_HoTen_QuanLyThuVien/
├── src/
│   ├── structures.h      ← Khai báo 4 CTDL + 3 struct dữ liệu
│   ├── functions.cpp     ← Cài đặt toàn bộ chức năng nghiệp vụ
│   └── main.cpp          ← Menu console, điều hướng người dùng
├── tests/
│   └── test_cases.cpp    ← 6 test cases, 23 assertions (23 PASS / 0 FAIL)
└── README.md
```

| File | Trách nhiệm | Lý do tách |
|------|-------------|------------|
| `structures.h` | Chỉ chứa định nghĩa CTDL và struct | Tái sử dụng được, include ở bất kỳ đâu |
| `functions.cpp` | Toàn bộ logic nghiệp vụ | Tách khỏi giao diện để viết test độc lập |
| `main.cpp` | Chỉ lo hiển thị menu và gọi hàm | Không chứa logic, dễ đọc và chỉnh sửa |
| `test_cases.cpp` | Test độc lập, dùng lại functions.cpp | Kiểm tra logic mà không cần chạy menu |

---

## 5. Các chức năng hệ thống

```
╔══════════════════════════════════════════╗
║     HỆ THỐNG QUẢN LÝ THƯ VIỆN           ║
╚══════════════════════════════════════════╝

1. Quản lý Sách
   ├── 1.1  Thêm sách mới         (kiểm tra ISBN trùng)
   ├── 1.2  Xóa sách              (từ chối nếu có người đang mượn)
   ├── 1.3  Tìm kiếm              (theo ISBN / tên / tác giả)
   └── 1.4  Xem toàn bộ kho sách

2. Quản lý Độc Giả
   ├── 2.1  Đăng ký độc giả mới  (kiểm tra mã trùng)
   ├── 2.2  Xem thông tin         (kèm danh sách sách đang mượn)
   └── 2.3  Xem tất cả độc giả

3. Mượn / Trả Sách
   ├── 3.1  Mượn sách             (tự vào hàng chờ nếu hết bản)
   ├── 3.2  Trả sách              (tự kích hoạt người đầu hàng chờ)
   └── 3.3  Xem danh sách đang mượn

4. Hàng Đợi Chờ Mượn
   ├── 4.1  Xem danh sách hàng chờ
   └── 4.2  Phục vụ người tiếp theo trong hàng

5. Lịch Sử & Undo
   ├── 5.1  Xem toàn bộ lịch sử giao dịch
   └── 5.2  Hoàn tác thao tác gần nhất

0. Thoát
```

---

## 6. Test cases

Chạy độc lập, không cần menu, kết quả: **23 PASS / 0 FAIL**

| # | Tình huống | Kiểm tra điều gì | Kết quả |
|---|------------|-----------------|---------|
| 1 | Thêm sách hợp lệ | Sách có trong LinkedList, ISBN có trong HashTable | ✅ PASS |
| 2 | Tìm ISBN không tồn tại | Trả về `nullptr`, không crash chương trình | ✅ PASS |
| 3 | Mượn khi `availableCopies == 0` | Tự động `enqueue` vào hàng chờ | ✅ PASS |
| 4 | Trả sách khi có người trong hàng chờ | `dequeue` và cấp sách cho người đầu | ✅ PASS |
| 5 | Undo thao tác mượn | `availableCopies` tăng lại, ISBN xóa khỏi `borrowedISBNs` | ✅ PASS |
| 6 | Xóa sách khi đang có người mượn | Hệ thống từ chối, sách vẫn trong kho | ✅ PASS |

---

## 7. Cách biên dịch và chạy

```bash
# Chạy chương trình chính
g++ -std=c++17 src/main.cpp src/functions.cpp -o library
./library

# Chạy test
g++ -std=c++17 tests/test_cases.cpp src/functions.cpp -o tests
./tests
```

## 8. Độ phức tạp (Time Complexity)

| Chức năng | Cấu trúc dữ liệu | Độ phức tạp |
|----------|----------------|------------|
| Thêm sách | Doubly Linked List | O(1) |
| Xóa sách | Doubly Linked List | O(n) |
| Tìm sách theo ISBN | Hash Table | O(1) (trung bình) |
| Tìm sách theo tên/tác giả | Linked List | O(n) |
| Đăng ký độc giả | Hash Table | O(1) |
| Tra cứu độc giả | Hash Table | O(1) |
| Mượn sách | Hash Table + Vector | O(1) |
| Trả sách | Vector | O(n) |
| Hàng chờ (enqueue/dequeue) | Queue | O(1) |
| Undo thao tác | Stack | O(1) |

---

### 📌 Nhận xét

- Hash Table giúp tăng tốc đáng kể các thao tác tra cứu
- Queue đảm bảo tính công bằng (FIFO)
- Stack phù hợp cho undo vì nguyên tắc LIFO
- Linked List tối ưu cho thêm/xóa nhưng tìm kiếm chậm hơn



---

## 9. Giới hạn hệ thống (Limitations)

Mặc dù hệ thống đã đáp ứng đầy đủ yêu cầu, vẫn còn một số giới hạn:

### ❌ 1. Không lưu trữ dữ liệu lâu dài
- Dữ liệu chỉ lưu trong RAM
- Khi tắt chương trình → mất toàn bộ dữ liệu

👉 Hướng phát triển:
- Lưu file `.txt` hoặc `.json`
- Kết nối database (MySQL, SQLite)

---

### ❌ 2. Chưa hỗ trợ đa người dùng
- Hệ thống chạy đơn luồng (single-thread)
- Không xử lý đồng thời nhiều người dùng

---

### ❌ 3. HashTable có thể giảm hiệu năng
- Khi nhiều phần tử → collision tăng
- Dùng chaining nên có thể thành O(n) trong worst-case

---

### ❌ 4. Undo chưa hỗ trợ tất cả thao tác
- Hiện tại:
  - ✔ Undo mượn sách
  - ✔ Undo cơ bản
- ❌ Chưa khôi phục lại sách đã xóa hoàn toàn

---

### ❌ 5. Giao diện console
- Chưa có GUI
- Trải nghiệm người dùng còn hạn chế

👉 Hướng phát triển:
- Làm GUI bằng Qt / WinForms / Web

---