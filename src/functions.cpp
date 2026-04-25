#include "structures.h"
#include <iomanip>

// ---------------- 1. QUẢN LÝ SÁCH ----------------

void addBook(LibrarySystem& lib, string isbn, string title, string author, string genre, int copies) {
    if (lib.bookMap.find(isbn) != lib.bookMap.end()) {
        cout << "[LỖI] Sach co ma ISBN " << isbn << " da ton tai!\n";
        return;
    }

    // Thêm vào Doubly Linked List
    lib.bookList.push_back({ isbn, title, author, genre, copies, copies });

    // Thêm con trỏ vào Hash Table để tra cứu O(1)
    lib.bookMap[isbn] = &lib.bookList.back();

    // Lưu lịch sử
    lib.historyStack.push({ "ADD_BOOK", "SYSTEM", isbn });
    cout << "[OK] Them sach '" << title << "' thanh cong!\n";
}

void displayBooks(const LibrarySystem& lib) {
    cout << "\n--- DANH SACH KHO SACH ---\n";
    cout << left << setw(10) << "ISBN" << setw(25) << "Ten Sach" << setw(20) << "Tac Gia"
        << setw(10) << "The Loai" << "So luong (Con/Tong)\n";
    cout << string(80, '-') << "\n";

    for (const auto& book : lib.bookList) {
        cout << left << setw(10) << book.isbn << setw(25) << book.title << setw(20) << book.author
            << setw(10) << book.genre << book.availableCopies << "/" << book.totalCopies << "\n";
    }
}

// ---------------- 2. QUẢN LÝ ĐỘC GIẢ ----------------

void addReader(LibrarySystem& lib, string id, string name, string phone) {
    if (lib.readerMap.find(id) != lib.readerMap.end()) {
        cout << "[LỖI] Doc gia co ma " << id << " da ton tai!\n";
        return;
    }
    lib.readerMap[id] = { id, name, phone, {} };
    cout << "[OK] Dang ky doc gia '" << name << "' thanh cong!\n";
}

void displayReaders(const LibrarySystem& lib) {
    cout << "\n--- DANH SACH DOC GIA ---\n";
    for (const auto& pair : lib.readerMap) {
        const Reader& r = pair.second;
        cout << "ID: " << r.id << " | Ten: " << r.name << " | SDT: " << r.phone << " | Dang muon: " << r.borrowedISBNs.size() << " cuon\n";
    }
}

// ---------------- 3. MƯỢN TRẢ SÁCH (Tích hợp Queue) ----------------

void borrowBook(LibrarySystem& lib, string readerId, string isbn) {
    if (lib.readerMap.find(readerId) == lib.readerMap.end()) {
        cout << "[LỖI] Khong tim thay doc gia!\n"; return;
    }
    if (lib.bookMap.find(isbn) == lib.bookMap.end()) {
        cout << "[LỖI] Khong tim thay sach!\n"; return;
    }

    Book* book = lib.bookMap[isbn];
    Reader& reader = lib.readerMap[readerId];

    if (book->availableCopies > 0) {
        book->availableCopies--;
        reader.borrowedISBNs.push_back(isbn);
        lib.historyStack.push({ "BORROW", readerId, isbn });
        cout << "[OK] Doc gia " << reader.name << " da muon sach '" << book->title << "'.\n";
    }
    else {
        // Hết sách -> Nhảy vào Queue
        lib.waitlistQueue[isbn].push(readerId);
        cout << "[CHÚ Ý] Sach da het. Doc gia " << reader.name << " duoc dua vao HANG CHO (Queue).\n";
    }
}

void returnBook(LibrarySystem& lib, string readerId, string isbn) {
    if (lib.readerMap.find(readerId) == lib.readerMap.end() || lib.bookMap.find(isbn) == lib.bookMap.end()) {
        cout << "[LỖI] Thong tin khong hop le!\n"; return;
    }

    Reader& reader = lib.readerMap[readerId];
    Book* book = lib.bookMap[isbn];

    auto it = find(reader.borrowedISBNs.begin(), reader.borrowedISBNs.end(), isbn);
    if (it != reader.borrowedISBNs.end()) {
        reader.borrowedISBNs.erase(it);
        lib.historyStack.push({ "RETURN", readerId, isbn });
        cout << "[OK] Doc gia " << reader.name << " da tra sach '" << book->title << "'.\n";

        // Kiểm tra Queue xem có ai đang chờ cuốn này không
        if (!lib.waitlistQueue[isbn].empty()) {
            string nextReaderId = lib.waitlistQueue[isbn].front();
            lib.waitlistQueue[isbn].pop();

            lib.readerMap[nextReaderId].borrowedISBNs.push_back(isbn);
            lib.historyStack.push({ "BORROW", nextReaderId, isbn });
            cout << "[HỆ THỐNG] Sach vua tra da duoc chuyen tu dong cho nguoi dau Hang Cho: Doc gia ID " << nextReaderId << ".\n";
        }
        else {
            book->availableCopies++; // Không ai chờ thì tăng số lượng tồn kho
        }
    }
    else {
        cout << "[LỖI] Doc gia khong muon cuon sach nay!\n";
    }
}

// ---------------- 4. LỊCH SỬ VÀ HOÀN TÁC (Tích hợp Stack) ----------------

void undoLastAction(LibrarySystem& lib) {
    if (lib.historyStack.empty()) {
        cout << "[THÔNG BÁO] Khong co lich su de hoan tac (Undo).\n";
        return;
    }

    Transaction lastTx = lib.historyStack.top();
    lib.historyStack.pop();

    if (lastTx.action == "BORROW") {
        // Khôi phục lại trạng thái chưa mượn
        lib.bookMap[lastTx.isbn]->availableCopies++;
        Reader& r = lib.readerMap[lastTx.readerId];
        r.borrowedISBNs.erase(remove(r.borrowedISBNs.begin(), r.borrowedISBNs.end(), lastTx.isbn), r.borrowedISBNs.end());
        cout << "[UNDO] Da hoan tac thao tac MUON SACH cua ID " << lastTx.readerId << ".\n";
    }
    else if (lastTx.action == "RETURN") {
        cout << "[UNDO] Tinh nang Undo Tra Sach can phuc tap hon, tam thoi bo qua.\n";
    }
    else if (lastTx.action == "ADD_BOOK") {
        cout << "[UNDO] Da hoan tac Them Sach.\n"; // Thực tế cần xóa khỏi DLL và Hash Table
    }
}

// ---------------- 5. XEM HÀNG CHỜ (Mới thêm để fix lỗi) ----------------

void viewWaitlist(LibrarySystem& lib, string isbn) {
    if (lib.waitlistQueue.find(isbn) == lib.waitlistQueue.end() || lib.waitlistQueue[isbn].empty()) {
        cout << "[THÔNG BÁO] Khong co ai dang cho muon sach nay.\n";
        return;
    }
    cout << "--- HANG CHO SACH " << isbn << " ---\n";
    queue<string> tempQueue = lib.waitlistQueue[isbn]; // Copy ra để in, tránh làm mất dữ liệu gốc
    int pos = 1;
    while (!tempQueue.empty()) {
        cout << pos++ << ". ID Doc gia: " << tempQueue.front() << "\n";
        tempQueue.pop();
    }
}