# 🎮 Maze Game (C++ CLI)

## 📝 Giới thiệu
Đây là chương trình game mê cung chạy trực tiếp trên giao diện dòng lệnh (Terminal/Console) được phát triển bằng ngôn ngữ **C++**. Dự án không chỉ là một trò chơi giải trí mà còn là sản phẩm thực hành áp dụng các cấu trúc dữ liệu và giải thuật cơ bản trong lập trình.

Người chơi có thể trải nghiệm việc tự giải mê cung thủ công hoặc sử dụng các thuật toán tự động để tìm đường thoát một cách thông minh.

---

## 🏗 Cấu trúc dữ liệu sử dụng
Dự án áp dụng linh hoạt các cấu trúc dữ liệu để tối ưu hóa hiệu suất:
* **Mảng 2D (`std::vector<std::vector>`):** Dùng để lưu trữ bản đồ mê cung, giúp truy xuất tọa độ (x, y) nhanh chóng.
* **Stack (LIFO):** Sử dụng trong thuật toán **DFS** để tạo mê cung ngẫu nhiên và giải mê cung theo cơ chế Backtracking (quay lui).
* **Queue (FIFO):** Sử dụng trong thuật toán **BFS** để duyệt theo từng lớp, đảm bảo luôn tìm ra đường đi ngắn nhất (Shortest Path).

---

## 🚀 Compile và Chạy
Để biên dịch tất cả các module và chạy chương trình, bạn sử dụng lệnh sau trong Terminal:

```bash
g++ -std=c++17 src/main.cpp src/maze.cpp src/player.cpp src/solver.cpp -o maze
./maze

### ✨ Chức năng chính
* **Tạo mê cung ngẫu nhiên:** Sử dụng thuật toán **DFS (Recursive Backtracker)** để tạo ra một mê cung hoàn hảo (*Perfect Maze*) - đảm bảo mọi ô đều có thể đi tới và luôn có đường thoát.
* **Chế độ chơi thủ công:** Người chơi điều khiển bằng các phím `W/A/S/D` để di chuyển trên giao diện ASCII.
* **Tự động giải bằng BFS:** Tìm và hiển thị đường đi ngắn nhất từ điểm bắt đầu đến đích.
* **Tự động giải bằng DFS:** Minh họa cơ chế hoạt động của **Backtracking**.
* **So sánh thuật toán:** Hiển thị và so sánh độ dài đường đi (*Distance*) giữa hai thuật toán BFS và DFS.

---

### 🧪 Test cases (Kịch bản kiểm thử)
1.  **Test 1 (Khởi tạo):** Kiểm tra mê cung được tạo có đúng kích thước $N \times M$ và có đầy đủ điểm Start/End.
2.  **Test 2 (Va chạm):** Kiểm tra người chơi không thể di chuyển xuyên qua tường hoặc ra ngoài biên.
3.  **Test 3 (BFS):** Đảm bảo thuật toán luôn tìm thấy lối ra ngắn nhất trong các mê cung có nhiều ngã rẽ.
4.  **Test 4 (DFS):** Kiểm tra khả năng quay lui chính xác và không bị lặp vô tận khi gặp các vòng lặp.
5.  **Test 5 (Hiệu năng):** So sánh số bước đi của hai thuật toán trên cùng một mê cung ngẫu nhiên.

##Cấu trúc thư mục
src/
 ├── main.cpp    : Quản lý menu và luồng điều khiển chính.
 ├── maze.cpp/h  : Thuật toán tạo mê cung và hiển thị ASCII.
 ├── player.cpp/h: Logic di chuyển và kiểm tra va chạm của người chơi.
 └── solver.cpp/h: Cài đặt thuật toán giải tự động (BFS, DFS).
tests/
 └── test_case.cpp: Các hàm kiểm thử tự động cho các chức năng của game.
docs/
 └── bao_cao.pdf : Tài liệu thuyết minh dự án (nếu có).
README.md        : Hướng dẫn và mô tả chi tiết dự án.
