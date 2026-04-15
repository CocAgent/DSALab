# 🗺️ Campus Pathfinder CLI

**Campus Pathfinder** là một ứng dụng giao diện dòng lệnh (CLI) được viết bằng C++, giúp sinh viên và khách tham quan tra cứu thông tin tòa nhà và tìm đường đi ngắn nhất trong khuôn viên trường học.

Dự án này được phát triển nhằm mục đích áp dụng các kiến thức về **Cấu trúc dữ liệu & Thuật toán (DSA)** vào một bài toán thực tế.

---

## ✨ Chức năng chính

- 🔍 **Tra cứu địa điểm (Lookup):** Tìm kiếm thông tin chi tiết (tên, mô tả) của một khu vực trong trường thông qua ID.
- 📍 **Tìm đường đi (Pathfinding):** Tính toán và in ra lộ trình ngắn nhất (ít bước di chuyển nhất) từ điểm A đến điểm B.
- 🗺️ **Bản đồ ảo (Virtual Graph):** Mô phỏng cấu trúc trường học dưới dạng đồ thị, cho phép tùy biến và mở rộng dễ dàng.

---

## 🧠 Cấu trúc dữ liệu & Thuật toán được sử dụng

Dự án áp dụng chặt chẽ các kiến thức DSA cốt lõi:

1. **Cây nhị phân tìm kiếm (Binary Search Tree - BST):**
   - *Mục đích:* Lưu trữ dữ liệu các địa điểm.
   - *Lý do:* Giúp thao tác tìm kiếm thông tin theo ID đạt độ phức tạp thời gian trung bình là $O(\log n)$, nhanh hơn nhiều so với việc dùng mảng hoặc danh sách liên kết.
2. **Đồ thị vô hướng & Danh sách kề (Adjacency List):**
   - *Mục đích:* Biểu diễn các con đường nối giữa các tòa nhà trong khuôn viên trường.
3. **Tìm kiếm theo chiều rộng (Breadth-First Search - BFS) & Queue:**
   - *Mục đích:* Tìm đường đi ngắn nhất giữa 2 điểm trên đồ thị không trọng số. Queue được dùng để quản lý thứ tự duyệt các node kề.
4. **Ngăn xếp (Stack):**
   - *Mục đích:* Truy vết ngược (Backtracking). Sau khi thuật toán BFS tìm được đích đến, Stack được dùng để lưu trữ đường đi từ đích ngược về nguồn. Khi `pop()` ra, ta sẽ có thứ tự di chuyển chuẩn xác từ đầu đến cuối.

---

## 🚀 Hướng dẫn cài đặt và chạy chương trình

### Yêu cầu hệ thống
- Môi trường có thể biên dịch C++ (G++, Clang, hoặc Visual Studio).
- Terminal hoặc Command Prompt.

### Các bước chạy
1. Clone repository này về máy hoặc tải file `main.cpp`.
2. Mở terminal tại thư mục chứa file code.
3. Biên dịch chương trình bằng lệnh:
   ```bash
   g++ main.cpp -o campus_app
