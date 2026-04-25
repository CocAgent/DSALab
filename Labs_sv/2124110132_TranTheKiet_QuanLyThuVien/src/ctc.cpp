#include "header.h"

static int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Gia tri nhap khong hop le. Vui long nhap lai.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static string readString(const string &prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void showBookMenu() {
    while (true) {
        cout << "\n-- Quan ly sach --\n";
        cout << "1. Them sach\n";
        cout << "2. Xoa sach\n";
        cout << "3. Cap nhat sach\n";
        cout << "4. Hien thi tat ca sach\n";
        cout << "5. Tim sach theo ten\n";
        cout << "0. Quay lai\n";
        int choice = readInt("Lua chon: ");
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                int id = readInt("Nhap ma sach: ");
                string title = readString("Nhap ten sach: ");
                string author = readString("Nhap tac gia: ");
                int year = readInt("Nhap nam xuat ban: ");
                addBook(id, title, author, year);
                break;
            }
            case 2: {
                int id = readInt("Nhap ma sach can xoa: ");
                removeBook(id);
                break;
            }
            case 3: {
                int id = readInt("Nhap ma sach can cap nhat: ");
                string title = readString("Nhap ten moi: ");
                string author = readString("Nhap tac gia moi: ");
                int year = readInt("Nhap nam xuat ban moi: ");
                updateBook(id, title, author, year);
                break;
            }
            case 4:
                displayBooks();
                break;
            case 5: {
                string title = readString("Nhap ten sach can tim: ");
                searchBookByTitle(title);
                break;
            }
            default:
                cout << "Lua chon khong hop le.\n";
                break;
        }
    }
}

void showMemberMenu() {
    while (true) {
        cout << "\n-- Quan ly thanh vien --\n";
        cout << "1. Them thanh vien\n";
        cout << "2. Hien thi tat ca thanh vien\n";
        cout << "0. Quay lai\n";
        int choice = readInt("Lua chon: ");
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                int id = readInt("Nhap ma thanh vien: ");
                string name = readString("Nhap ten thanh vien: ");
                addMember(id, name);
                break;
            }
            case 2:
                displayMembers();
                break;
            default:
                cout << "Lua chon khong hop le.\n";
                break;
        }
    }
}

void showLoanMenu() {
    while (true) {
        cout << "\n-- Quan ly muon tra --\n";
        cout << "1. Muon sach truc tiep\n";
        cout << "2. Tra sach\n";
        cout << "3. Them yeu cau muon sach vao hang doi\n";
        cout << "4. Xu ly yeu cau tiep theo\n";
        cout << "0. Quay lai\n";
        int choice = readInt("Lua chon: ");
        if (choice == 0) break;
        switch (choice) {
            case 1: {
                int memberId = readInt("Nhap ma thanh vien: ");
                int bookId = readInt("Nhap ma sach: ");
                borrowBook(memberId, bookId);
                break;
            }
            case 2: {
                int memberId = readInt("Nhap ma thanh vien: ");
                int bookId = readInt("Nhap ma sach: ");
                returnBook(memberId, bookId);
                break;
            }
            case 3: {
                int memberId = readInt("Nhap ma thanh vien: ");
                int bookId = readInt("Nhap ma sach: ");
                enqueueRequest(memberId, bookId);
                break;
            }
            case 4:
                processNextRequest();
                break;
            default:
                cout << "Lua chon khong hop le.\n";
                break;
        }
    }
}

void showMenu() {
    initializeLibrary();
    while (true) {
        cout << "\n==== HE THONG QUAN LY THU VIEN ====" << "\n";
        cout << "1. Quan ly sach\n";
        cout << "2. Quan ly thanh vien\n";
        cout << "3. Quan ly muon tra\n";
        cout << "4. Hoan tac hanh dong gan nhat\n";
        cout << "0. Thoat\n";
        int choice = readInt("Lua chon chuc nang: ");
        switch (choice) {
            case 1:
                showBookMenu();
                break;
            case 2:
                showMemberMenu();
                break;
            case 3:
                showLoanMenu();
                break;
            case 4:
                undoLastOperation();
                break;
            case 0:
                cout << "Cam on ban da su dung chuong trinh.\n";
                return;
            default:
                cout << "Lua chon khong hop le.\n";
                break;
        }
    }
}

int main() {
    showMenu();
    return 0;
}
