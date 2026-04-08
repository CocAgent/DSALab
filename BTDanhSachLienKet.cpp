#include <iostream>
#include <cmath>
using namespace std;

// ================= NODE =================
struct Node {
    int data;
    Node* next;
};

// ================= LIST =================
struct List {
    Node* head;
    Node* tail;
};

// ================= KHOI TAO =================
void KhoiTaoDSLK(List& L) {
    L.head = L.tail = nullptr;
}

Node* TaoNode(int x) {
    Node* p = new Node;
    p->data = x;
    p->next = nullptr;
    return p;
}

bool DanhSachRong(const List& L) {
    return L.head == nullptr;
}

// ================= THEM =================

// 1. Them dau
void ThemDau(List& L, int x) {
    Node* p = TaoNode(x);
    if (DanhSachRong(L)) L.head = L.tail = p;
    else {
        p->next = L.head;
        L.head = p;
    }
}

// 2. Them cuoi
void ThemCuoi(List& L, int x) {
    Node* p = TaoNode(x);
    if (DanhSachRong(L)) L.head = L.tail = p;
    else {
        L.tail->next = p;
        L.tail = p;
    }
}

// 3. Them sau q
void ThemSau(List& L, Node* q, int x) {
    if (q == nullptr) return;
    Node* p = TaoNode(x);
    p->next = q->next;
    q->next = p;
    if (q == L.tail) L.tail = p;
}

// 4. Them truoc q
void ThemTruoc(List& L, Node* q, int x) {
    if (q == nullptr) return;
    if (q == L.head) {
        ThemDau(L, x);
        return;
    }
    Node* p = TaoNode(x);
    Node* cur = L.head;
    while (cur->next != q) cur = cur->next;
    p->next = q;
    cur->next = p;
}

// ================= XOA =================

// 5. Xoa dau
void XoaDau(List& L) {
    if (DanhSachRong(L)) return;
    Node* p = L.head;
    L.head = L.head->next;
    if (L.head == nullptr) L.tail = nullptr;
    delete p;
}

// 6. Xoa cuoi
void XoaCuoi(List& L) {
    if (DanhSachRong(L)) return;
    if (L.head == L.tail) {
        delete L.head;
        L.head = L.tail = nullptr;
        return;
    }
    Node* cur = L.head;
    while (cur->next != L.tail) cur = cur->next;
    delete L.tail;
    L.tail = cur;
    L.tail->next = nullptr;
}

// 7. Xoa gia tri x
bool XoaGiaTri(List& L, int x) {
    if (DanhSachRong(L)) return false;
    Node* cur = L.head, * prev = nullptr;

    while (cur && cur->data != x) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) return false;

    if (!prev) XoaDau(L);
    else {
        prev->next = cur->next;
        if (cur == L.tail) L.tail = prev;
        delete cur;
    }
    return true;
}

void XoaDanhSach(List& L) {
    while (!DanhSachRong(L)) XoaDau(L);
}

// ================= 8. DUYET =================
void InDanhSach(const List& L) {
    if (DanhSachRong(L)) {
        cout << "  [Danh sach rong]\n";
        return;
    }
    cout << "  HEAD -> ";
    Node* cur = L.head;
    while (cur != nullptr) {
        cout << cur->data;
        if (cur->next) cout << " -> ";
        cur = cur->next;
    }
    cout << " -> NULL\n";
}

// 9. Tim max
int TimMax(const List& L) {
    if (DanhSachRong(L)) return -1;
    int mx = L.head->data;
    Node* cur = L.head;
    while (cur) {
        if (cur->data > mx) mx = cur->data;
        cur = cur->next;
    }
    return mx;
}

// 10. Tim x
Node* TimX(const List& L, int x) {
    Node* cur = L.head;
    while (cur) {
        if (cur->data == x) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// 11. Tim chan dau
int TimChanDau(const List& L) {
    Node* cur = L.head;
    while (cur) {
        if (cur->data % 2 == 0) return cur->data;
        cur = cur->next;
    }
    return -1;
}

// 12. Tim chan cuoi
int TimChanCuoi(const List& L) {
    int kq = -1;
    Node* cur = L.head;
    while (cur) {
        if (cur->data % 2 == 0) kq = cur->data;
        cur = cur->next;
    }
    return kq;
}

// 13. Liet ke am
void LietKeAm(const List& L) {
    Node* cur = L.head;
    while (cur) {
        if (cur->data < 0) cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

// 14. Tong duong
long long TongDuong(const List& L) {
    long long s = 0;
    Node* cur = L.head;
    while (cur) {
        if (cur->data > 0) s += cur->data;
        cur = cur->next;
    }
    return s;
}

// 15. Dem am
int DemAm(const List& L) {
    int dem = 0;
    Node* cur = L.head;
    while (cur) {
        if (cur->data < 0) dem++;
        cur = cur->next;
    }
    return dem;
}

// 16. So chinh phuong
bool LaChinhPhuong(int n) {
    if (n < 0) return false;
    int s = sqrt(n);
    return s * s == n;
}

bool CoSoChinhPhuong(const List& L) {
    Node* cur = L.head;
    while (cur) {
        if (LaChinhPhuong(cur->data)) return true;
        cur = cur->next;
    }
    return false;
}

// 17. Dem cuc dai
int DemCucDai(const List& L) {
    if (DanhSachRong(L)) return 0;
    int mx = TimMax(L);
    int dem = 0;
    Node* cur = L.head;
    while (cur) {
        if (cur->data == mx) dem++;
        cur = cur->next;
    }
    return dem;
}

// ================= MAIN TEST =================
int main() {
    List L;
    KhoiTaoDSLK(L);

    ThemCuoi(L, 10);
    ThemCuoi(L, -5);
    ThemCuoi(L, 20);
    ThemCuoi(L, 16);
    ThemCuoi(L, -2);

    InDanhSach(L);

    cout << "Max: " << TimMax(L) << endl;
    cout << "Chan dau: " << TimChanDau(L) << endl;
    cout << "Chan cuoi: " << TimChanCuoi(L) << endl;
    cout << "Tong duong: " << TongDuong(L) << endl;
    cout << "Dem am: " << DemAm(L) << endl;
    cout << "Co chinh phuong: " << (CoSoChinhPhuong(L) ? "Co" : "Khong") << endl;
    cout << "So cuc dai: " << DemCucDai(L) << endl;

    return 0;
}