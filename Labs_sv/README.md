# Maze Game (C++ CLI)

## Giới thiệu

Đây là chương trình game mê cung chạy trên terminal được viết bằng C++.

Người chơi có thể tự di chuyển hoặc để chương trình tự động giải mê cung bằng các thuật toán.

---

## Chức năng chính

1. Tạo mê cung ngẫu nhiên bằng DFS (Recursive Backtracker)
2. Hiển thị mê cung dạng ASCII
3. Người chơi di chuyển bằng W/A/S/D
4. Giải mê cung bằng BFS (đường ngắn nhất)
5. Giải mê cung bằng DFS (có backtracking)
6. So sánh độ dài đường đi giữa BFS và DFS

---

## Cấu trúc dữ liệu sử dụng

* **Mảng 2D**: lưu mê cung
* **Stack**: dùng cho DFS (generate + solve)
* **Queue**: dùng cho BFS

---

## Thuật toán

### DFS (Recursive Backtracker)

* Dùng để tạo mê cung
* Đi sâu và quay lui (backtracking)
* Đảm bảo mê cung có đường đi

### BFS

* Duyệt theo từng lớp
* Đảm bảo tìm đường ngắn nhất

### DFS (Solve)

* Duyệt sâu trước
* Không đảm bảo đường ngắn nhất

---

## Cách chạy chương trình

```bash
g++ -std=c++17 *.cpp -o maze
./maze
```

---

## Test case

1. Tạo mê cung ngẫu nhiên
2. Tìm đường bằng BFS
3. Tìm đường bằng DFS
4. Kiểm tra di chuyển người chơi
5. So sánh BFS và DFS

---

## Cấu trúc project

* main.cpp: điều khiển chương trình
* maze.cpp/h: tạo và hiển thị mê cung
* player.cpp/h: xử lý người chơi
* solver.cpp/h: BFS và DFS
