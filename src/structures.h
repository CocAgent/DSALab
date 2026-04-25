#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// Dữ liệu Sách
struct Book {
    string isbn;
    string title;
    string author;
    string genre;
    int totalCopies = 0;       // Đã fix cảnh báo uninitialized
    int availableCopies = 0;   // Đã fix cảnh báo uninitialized
};

// Dữ liệu Độc giả
struct Reader {
    string id;
    string name;
    string phone;
    vector<string> borrowedISBNs;
};

// Dữ liệu Lịch sử giao dịch
struct Transaction {
    string action; // "BORROW", "RETURN", "ADD_BOOK"
    string readerId;
    string isbn;
};

// Hệ thống chính quản lý 4 Cấu trúc dữ liệu (CTDL)
struct LibrarySystem {
    // 1. Doubly Linked List (list): Lưu kho sách để thêm/xóa O(1)
    list<Book> bookList;

    // 2. Hash Table (unordered_map): Tìm kiếm sách và độc giả O(1)
    unordered_map<string, Book*> bookMap;
    unordered_map<string, Reader> readerMap;

    // 3. Queue (queue): Hàng đợi mượn sách khi sách hết bản (FIFO)
    unordered_map<string, queue<string>> waitlistQueue; // Key: ISBN, Value: Hàng đợi ReaderID

    // 4. Stack (stack): Lưu lịch sử giao dịch để Undo (LIFO)
    stack<Transaction> historyStack;
};

// Khai báo các hàm xử lý
void addBook(LibrarySystem& lib, string isbn, string title, string author, string genre, int copies);
void displayBooks(const LibrarySystem& lib);
void addReader(LibrarySystem& lib, string id, string name, string phone);
void displayReaders(const LibrarySystem& lib);
void borrowBook(LibrarySystem& lib, string readerId, string isbn);
void returnBook(LibrarySystem& lib, string readerId, string isbn);
void viewWaitlist(LibrarySystem& lib, string isbn);
void undoLastAction(LibrarySystem& lib);