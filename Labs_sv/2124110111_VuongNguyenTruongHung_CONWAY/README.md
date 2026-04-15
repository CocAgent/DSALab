# Recommendation Engine

## Thông tin sinh viên

- Họ tên: Vương Nguyễn Trường Hưng
- MSSV: 2124110111
- Đề tài: Conway: game of life

Ứng dụng C++ CLI mô phỏng hệ thống gợi ý đơn giản cho phim / bài hát / sản phẩm. Chương trình ghi nhận lịch sử tương tác của người dùng, phân tích độ tương đồng, sau đó đề xuất item phù hợp theo hai hướng:

- Collaborative Filtering: người dùng tương tự cùng thích những item nào.
- Item-based Recommendation: item giống với những item bạn đã thích.

## Cấu trúc dữ liệu sử dụng

- `Graph`:
  Dùng adjacency list để lưu quan hệ user-item. Mỗi cạnh thể hiện mức độ quan tâm của user với item, phù hợp vì bài toán recommendation là bài toán liên kết giữa hai tập đỉnh.
- `BST`:
  Dùng để lưu catalog item theo `id`, hỗ trợ tìm kiếm item nhanh hơn danh sách tuyến tính và để duyệt in-order có thứ tự.
- `Priority Queue (Heap)`:
  Dùng để xếp hạng gợi ý theo điểm relevance và lấy Top-N nhanh, đáp ứng yêu cầu cấu trúc dữ liệu dạng Heap.
- `unordered_map`:
  Dùng để lưu lịch sử tương tác của user và chỉ mục item-user, truy cập trung bình O(1).

## Chức năng

1. Thêm người dùng mới.
2. Thêm item vào catalog.
3. Ghi nhận hành động `view`, `like`, `rating`.
4. Tìm item trong BST theo ID.
5. Hiển thị catalog.
6. Gợi ý theo người dùng tương tự.
7. Gợi ý theo item tương tự.
8. Hiển thị Top-N gợi ý tổng hợp.
9. Báo cáo thống kê.
10. Xem lịch sử người dùng.
11. Nạp dữ liệu mẫu để demo.

## Thiết kế file

```text
src/
  main.cpp       - Menu và điều khiển chương trình
  structures.h   - Khai báo struct, class, prototype
  functions.cpp  - Cài đặt các cấu trúc dữ liệu và thuật toán
tests/
  test_cases.cpp - Bộ test cơ bản và edge cases
docs/
  .gitkeep
README.md        - Mô tả ứng dụng và hướng dẫn chạy
```

## Compile và chạy

Build chương trình chính:

```bash
g++ -std=c++17 src/main.cpp src/functions.cpp -o app
./app
```

Build bộ test:

```bash
g++ -std=c++17 tests/test_cases.cpp src/functions.cpp -o test_app
./test_app
```

## Test cases chính

1. Thêm user và item, kiểm tra catalog được sắp xếp theo BST.
2. Tìm kiếm item tồn tại / không tồn tại.
3. Ghi nhận `view`, `like`, `rating` và kiểm tra số liệu cập nhật đúng.
4. Tạo gợi ý trên dữ liệu mẫu và đảm bảo có kết quả hợp lệ.
5. Test edge case: ID rỗng, user/item không tồn tại, rating ngoài miền hợp lệ.
6. Test thống kê item phổ biến và user hoạt động.
