# 🏛️ Quản Lý Thư Viện

📌 Giới thiệu
Đây là chương trình hệ thống quản lý thư viện được viết bằng C++. Chương trình giúp người dùng:

- Quản lý sách trong thư viện
- Quản lý thành viên
- Thực hiện các thao tác mượn/trả sách
- Xử lý các yêu cầu liên quan

🚀 Công nghệ sử dụng
Ngôn ngữ: C++

Cấu trúc dữ liệu:
- Danh sách liên kết (Linked List) cho sách và thành viên
- Cấu trúc dữ liệu cơ bản cho quản lý

Thuật toán:
- Tìm kiếm tuyến tính
- Quản lý danh sách động

📂 Cấu trúc thư mục
```
Labs_sv/
└── 2124110132_TranTheKiet_QuanLyThuVien/
    ├── src/
    │   ├── ctc.cpp          (Hàm main và menu điều khiển)
    │   ├── header.h         (Khai báo cấu trúc dữ liệu và hàm)
    │   └── caidat.cpp       (Cài đặt chi tiết các hàm chức năng)
    ├── tests/
    │   └── test_cases.cpp   (Các kịch bản kiểm thử chương trình)
    ├── docs/
    │   └── bao_cao.pdf      (Tài liệu giải thích thuật toán)
    └── README.md            (Hướng dẫn này)
```

⚙️ Cách chạy chương trình
🔹 Biên dịch
```
g++ src/caidat.cpp src/ctc.cpp -o library_manager
```
🔹 Chạy
```
./library_manager
```

🧪 Chạy test
```
g++ src/caidat.cpp tests/test_cases.cpp -o test
./test
```

🎯 Chức năng chính
1. Quản lý sách
   - Thêm sách mới
   - Xóa sách
   - Tìm kiếm sách

2. Quản lý thành viên
   - Thêm thành viên
   - Xóa thành viên

3. Quản lý mượn/trả
   - Mượn sách
   - Trả sách
   - Xử lý yêu cầu

👤 Thông tin sinh viên
Họ tên: Trần Thế Kiệt
MSSV: 2124110132

📌 Ghi chú
- Chương trình chạy trên môi trường console
- Sử dụng encoding UTF-8 để hiển thị tiếng Việt
- Tài liệu chi tiết trong docs/bao_cao.pdf

💡 Hướng phát triển
- Thêm giao diện đồ họa (GUI)
- Lưu dữ liệu vào file
- Thêm chức năng thống kê và báo cáo