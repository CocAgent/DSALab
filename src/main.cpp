#include "structures.h"

void inMenu() {
    cout << "\n============================================\n";
    cout << "      HE THONG QUAN LY THU VIEN (C++17)       \n";
    cout << "============================================\n";
    cout << "1. Xem danh sach kho sach\n";
    cout << "2. Them sach moi\n";
    cout << "3. Xem danh sach doc gia\n";
    cout << "4. Dang ky doc gia moi\n";
    cout << "5. Muon sach\n";
    cout << "6. Tra sach\n";
    cout << "7. Undo (Hoan tac thao tac truoc do)\n";
    cout << "0. Thoat chuong trinh\n";
    cout << "============================================\n";
    cout << "Nhap lua chon: ";
}

int main() {
    LibrarySystem lib;

    // --- MOCK DATA (Dữ liệu tạo sẵn để Test) ---
    addBook(lib, "B001", "De Men Phieu Luu Ky", "To Hoai", "Van hoc", 2);
    addBook(lib, "B002", "So Do", "Vu Trong Phung", "Van hoc", 1);
    addBook(lib, "B003", "Clean Code", "Robert C. Martin", "IT", 3);

    addReader(lib, "R001", "Nguyen Van A", "0901234567");
    addReader(lib, "R002", "Tran Thi B", "0987654321");
    addReader(lib, "R003", "Le Van C", "0912345678");
    // -------------------------------------------

    int choice;
    do {
        inMenu();
        cin >> choice;
        cin.ignore(); // Xóa bộ đệm

        switch (choice) {
        case 1:
            displayBooks(lib);
            break;
        case 2: {
            string isbn, title, author, genre;
            int copies;
            cout << "Nhap ISBN: "; getline(cin, isbn);
            cout << "Nhap Ten Sach: "; getline(cin, title);
            cout << "Nhap Tac Gia: "; getline(cin, author);
            cout << "Nhap The Loai: "; getline(cin, genre);
            cout << "Nhap So luong: "; cin >> copies;
            addBook(lib, isbn, title, author, genre, copies);
            break;
        }
        case 3:
            displayReaders(lib);
            break;
        case 4: {
            string id, name, phone;
            cout << "Nhap ID Doc Gia: "; getline(cin, id);
            cout << "Nhap Ten: "; getline(cin, name);
            cout << "Nhap SDT: "; getline(cin, phone);
            addReader(lib, id, name, phone);
            break;
        }
        case 5: {
            string rId, bIsbn;
            cout << "Nhap ID Doc Gia muon: "; getline(cin, rId);
            cout << "Nhap ISBN sach can muon: "; getline(cin, bIsbn);
            borrowBook(lib, rId, bIsbn);
            break;
        }
        case 6: {
            string rId, bIsbn;
            cout << "Nhap ID Doc Gia tra: "; getline(cin, rId);
            cout << "Nhap ISBN sach can tra: "; getline(cin, bIsbn);
            returnBook(lib, rId, bIsbn);
            break;
        }
        case 7:
            undoLastAction(lib);
            break;
        case 0:
            cout << "Dang thoat he thong...\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}