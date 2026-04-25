#include "../src/structures.h" // Trỏ đúng về thư mục src chứa cấu trúc
#include <iostream>
#include <cassert>

using namespace std;

// Hàm tiện ích để in kết quả test
void printTestResult(const string& testName, bool passed) {
    if (passed) {
        cout << "[ PASS ] " << testName << "\n";
    }
    else {
        cout << "[ FAIL ] " << testName << " <--- LỖI TẠI ĐÂY\n";
    }
}

// ---------------- CÁC TEST CASES ----------------

// Test 1: Thêm sách hợp lệ (Kiểm tra Doubly Linked List & Hash Table)
void test_AddValidBook() {
    LibrarySystem lib;
    addBook(lib, "ISBN-001", "C++ Primer", "Stanley", "IT", 5);

    bool passed = (lib.bookMap.find("ISBN-001") != lib.bookMap.end()) &&
        (lib.bookMap["ISBN-001"]->availableCopies == 5);
    printTestResult("TC01: Them sach hop le vao he thong", passed);
}

// Test 2: Edge Case - Không cho phép thêm sách trùng mã ISBN
void test_AddDuplicateBook() {
    LibrarySystem lib;
    addBook(lib, "ISBN-001", "C++ Primer", "Stanley", "IT", 5);
    size_t initialSize = lib.bookList.size();

    // Cố tình thêm trùng mã
    cout << "  -> Ky vong in ra loi: ";
    addBook(lib, "ISBN-001", "C++ Nang cao", "N/A", "IT", 10);

    bool passed = (lib.bookList.size() == initialSize); // Kích thước kho không đổi
    printTestResult("TC02: Tu choi them sach trung ma ISBN", passed);
}

// Test 3: Mượn sách thành công và lưu lịch sử (Kiểm tra Stack)
void test_BorrowBookSuccess() {
    LibrarySystem lib;
    addBook(lib, "B1", "Sach 1", "TG 1", "TL", 1);
    addReader(lib, "R1", "Nguyen A", "0123");

    borrowBook(lib, "R1", "B1");

    bool passed = (lib.bookMap["B1"]->availableCopies == 0) && // Sách đã giảm số lượng
        (lib.readerMap["R1"].borrowedISBNs.size() == 1) && // Độc giả đã cầm sách
        (!lib.historyStack.empty()); // Lịch sử đã lưu
    printTestResult("TC03: Muon sach thanh cong va ghi nhan Stack", passed);
}

// Test 4: Edge Case - Mượn sách khi đã hết bản (Kiểm tra Queue FIFO)
void test_BorrowWhenEmpty_TriggersQueue() {
    LibrarySystem lib;
    addBook(lib, "B1", "Sach Hiem", "TG", "TL", 1); // Chỉ có 1 bản
    addReader(lib, "R1", "Doc Gia 1", "012");
    addReader(lib, "R2", "Doc Gia 2", "013");

    borrowBook(lib, "R1", "B1"); // R1 mượn -> Hết sách

    cout << "  -> Ky vong dua R2 vao hang cho: ";
    borrowBook(lib, "R2", "B1"); // R2 mượn tiếp -> Phải vào Queue

    bool passed = (!lib.waitlistQueue["B1"].empty()) &&
        (lib.waitlistQueue["B1"].front() == "R2");
    printTestResult("TC04: Dua doc gia vao Queue khi sach het ban", passed);
}

// Test 5: Trả sách tự động cấp cho người trong hàng chờ
void test_ReturnBook_AutoAssignFromQueue() {
    LibrarySystem lib;
    addBook(lib, "B1", "Sach Hiem", "TG", "TL", 1);
    addReader(lib, "R1", "Doc Gia 1", "012");
    addReader(lib, "R2", "Doc Gia 2", "013");

    borrowBook(lib, "R1", "B1"); // R1 mượn
    borrowBook(lib, "R2", "B1"); // R2 vào Queue

    cout << "  -> Ky vong tu dong cap sach cho R2: ";
    returnBook(lib, "R1", "B1"); // R1 trả sách

    // Lúc này R2 phải tự động có sách, và Queue phải trống
    bool passed = (lib.waitlistQueue["B1"].empty()) &&
        (lib.readerMap["R2"].borrowedISBNs.size() == 1);
    printTestResult("TC05: Tra sach tu dong cap cho nguoi dau Queue", passed);
}

// Test 6: Undo thao tác mượn sách
void test_UndoBorrow() {
    LibrarySystem lib;
    addBook(lib, "B1", "Sach 1", "TG", "TL", 5);
    addReader(lib, "R1", "Doc Gia 1", "012");

    borrowBook(lib, "R1", "B1"); // availableCopies = 4

    cout << "  -> Ky vong hoan tac: ";
    undoLastAction(lib); // Hoàn tác thao tác mượn

    bool passed = (lib.bookMap["B1"]->availableCopies == 5) && // Trả lại kho
        (lib.readerMap["R1"].borrowedISBNs.empty()); // Độc giả không còn cầm sách
    printTestResult("TC06: Hoan tac (Undo) thao tac muon sach", passed);
}

// ---------------- HÀM MAIN CHẠY TEST ----------------
int main() {
    cout << "============================================\n";
    cout << "      CHAY KIEM THU TU DONG (UNIT TESTS)    \n";
    cout << "============================================\n";

    test_AddValidBook();
    cout << "--------------------------------------------\n";
    test_AddDuplicateBook();
    cout << "--------------------------------------------\n";
    test_BorrowBookSuccess();
    cout << "--------------------------------------------\n";
    test_BorrowWhenEmpty_TriggersQueue();
    cout << "--------------------------------------------\n";
    test_ReturnBook_AutoAssignFromQueue();
    cout << "--------------------------------------------\n";
    test_UndoBorrow();

    cout << "============================================\n";
    cout << "        HOAN TAT KIEM THU (6/6 TESTS)       \n";
    cout << "============================================\n";

    return 0;
}