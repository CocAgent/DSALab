#include "../src/header.h"
#include <iostream>
#include <cassert>
#include <string>

// Cần khai báo extern hoặc include caidat.cpp khi biên dịch để liên kết dữ liệu
// Trong bài này, chúng ta sẽ giả định biên dịch chung: g++ tests/test_cases.cpp src/caidat.cpp

void testAddBook() {
    initializeLibrary();
    addBook(1, "Lap trinh C++", "Tac Gia A", 2023);
    
    Book* b = findBook(1);
    assert(b != nullptr); // Kiểm tra sách có tồn tại không
    assert(b->title == "Lap trinh C++"); // Kiểm tra đúng tên sách không
    
    // Kiểm tra trùng ID
    addBook(1, "Sach Trung ID", "Tac Gia B", 2024);
    // (Hàm addBook của bạn sẽ in ra thông báo lỗi nhưng không thêm vào list)
    
    std::cout << "1. Kiem tra them sach: THANH CONG" << std::endl;
}

void testSearchBook() {
    initializeLibrary();
    addBook(10, "Cau truc du lieu", "Thay Kiet", 2024);
    addBook(20, "Laravel Framework", "Tran The Kiet", 2026);
    
    // Kiểm tra tìm kiếm qua BST (hàm searchBookByTitle in ra màn hình)
    // Ở đây ta test logic findBook
    assert(findBook(20)->author == "Tran The Kiet");
    
    std::cout << "2. Kiem tra tim sach: THANH CONG" << std::endl;
}

void testUndoFunction() {
    initializeLibrary();
    addMember(1, "Nguyen Van A");
    
    // Trước khi undo, member phải tồn tại
    assert(findMember(1) != nullptr);
    
    // Thực hiện undo
    undoLastOperation();
    
    // Sau khi undo, member ID 1 phải bị xóa (theo logic trong caidat.cpp )
    assert(findMember(1) == nullptr);
    
    std::cout << "3. Kiem tra Hoan tac (Undo): THANH CONG" << std::endl;
}

void testQueueBorrow() {
    initializeLibrary();
    addBook(100, "Clean Code", "Uncle Bob", 2008);
    addMember(5, "Sinh Vien It");
    
    // Thêm vào hàng đợi
    enqueueRequest(5, 100);
    
    // Xử lý yêu cầu
    processNextRequest();
    
    // Kiểm tra sách đã được mượn chưa
    Book* b = findBook(100);
    assert(b->borrowed == true);
    assert(b->borrowedBy == 5);
    
    std::cout << "4. Kiem tra Hang doi (Queue): THANH CONG" << std::endl;
}

int main() {
    std::cout << "--- BAT DAU KIEM THU HE THONG ---" << std::endl;
    
    testAddBook();
    testSearchBook();
    testUndoFunction();
    testQueueBorrow();
    
    std::cout << "---------------------------------" << std::endl;
    std::cout << "TAT CA KIEM TRA DA VUOT QUA!" << std::endl;
    
    return 0;
}