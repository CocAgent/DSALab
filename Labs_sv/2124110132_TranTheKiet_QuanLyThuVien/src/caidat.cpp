#include "header.h"

static BookNode *bookHead = nullptr;
static MemberNode *memberHead = nullptr;
static OpNode *opStackTop = nullptr;
static BorrowRequest *requestHead = nullptr;
static BorrowRequest *requestTail = nullptr;
static BSTNode *bookIndexRoot = nullptr;

static void pushOperation(const Operation &op) {
    OpNode *node = new OpNode{op, opStackTop};
    opStackTop = node;
}

static bool popOperation(Operation &op) {
    if (!opStackTop) return false;
    OpNode *node = opStackTop;
    op = node->data;
    opStackTop = node->next;
    delete node;
    return true;
}

static void enqueueRequestNode(int memberId, int bookId) {
    BorrowRequest *node = new BorrowRequest{memberId, bookId, nullptr};
    if (!requestHead) {
        requestHead = requestTail = node;
    } else {
        requestTail->next = node;
        requestTail = node;
    }
}

static BorrowRequest *dequeueRequestNode() {
    if (!requestHead) return nullptr;
    BorrowRequest *node = requestHead;
    requestHead = requestHead->next;
    if (!requestHead) requestTail = nullptr;
    return node;
}

static BSTNode *createBSTNode(Book *book) {
    BSTNode *node = new BSTNode;
    node->book = book;
    node->left = node->right = nullptr;
    return node;
}

static BSTNode *insertBST(BSTNode *root, Book *book) {
    if (!root) return createBSTNode(book);
    if (book->title < root->book->title) {
        root->left = insertBST(root->left, book);
    } else {
        root->right = insertBST(root->right, book);
    }
    return root;
}

static void clearBST(BSTNode *root) {
    if (!root) return;
    clearBST(root->left);
    clearBST(root->right);
    delete root;
}

static void rebuildBookIndex() {
    clearBST(bookIndexRoot);
    bookIndexRoot = nullptr;
    BookNode *current = bookHead;
    while (current) {
        bookIndexRoot = insertBST(bookIndexRoot, &current->data);
        current = current->next;
    }
}

static Book *searchBST(BSTNode *root, const string &title) {
    if (!root) return nullptr;
    if (title == root->book->title) return root->book;
    if (title < root->book->title) return searchBST(root->left, title);
    return searchBST(root->right, title);
}

void initializeLibrary() {
    bookHead = nullptr;
    memberHead = nullptr;
    opStackTop = nullptr;
    requestHead = requestTail = nullptr;
    clearBST(bookIndexRoot);
    bookIndexRoot = nullptr;
}

void addBook(int id, const string &title, const string &author, int year) {
    if (findBook(id)) {
        cout << "Ma sach da ton tai.\n";
        return;
    }
    Book book = {id, title, author, year, false, 0};
    BookNode *node = new BookNode{book, nullptr};
    if (!bookHead) {
        bookHead = node;
    } else {
        BookNode *current = bookHead;
        while (current->next) current = current->next;
        current->next = node;
    }
    rebuildBookIndex();
    pushOperation(Operation{Operation::ADD_BOOK, id, 0});
    cout << "Da them sach.\n";
}

void removeBook(int id) {
    BookNode *current = bookHead;
    BookNode *previous = nullptr;
    while (current && current->data.id != id) {
        previous = current;
        current = current->next;
    }
    if (!current) {
        cout << "Khong tim thay sach.\n";
        return;
    }
    if (current->data.borrowed) {
        cout << "Sach dang duoc muon, khong the xoa.\n";
        return;
    }
    if (!previous) {
        bookHead = current->next;
    } else {
        previous->next = current->next;
    }
    delete current;
    rebuildBookIndex();
    pushOperation(Operation{Operation::REMOVE_BOOK, id, 0});
    cout << "Da xoa sach.\n";
}

void updateBook(int id, const string &newTitle, const string &newAuthor, int newYear) {
    Book *book = findBook(id);
    if (!book) {
        cout << "Khong tim thay sach.\n";
        return;
    }
    book->title = newTitle;
    book->author = newAuthor;
    book->year = newYear;
    rebuildBookIndex();
    cout << "Da cap nhat thong tin sach.\n";
}

void displayBooks() {
    if (!bookHead) {
        cout << "Chua co sach trong thu vien.\n";
        return;
    }
    cout << "\nDanh sach sach:\n";
    cout << "ID\tTen sach\tTac gia\tNam xuat ban\tTinh trang\n";
    BookNode *current = bookHead;
    while (current) {
        cout << current->data.id << "\t" << current->data.title << "\t"
             << current->data.author << "\t" << current->data.year << "\t"
             << (current->data.borrowed ? "Da muon" : "Con sach") << "\n";
        current = current->next;
    }
}

Book *findBook(int id) {
    BookNode *current = bookHead;
    while (current) {
        if (current->data.id == id) return &current->data;
        current = current->next;
    }
    return nullptr;
}

void searchBookByTitle(const string &title) {
    Book *book = searchBST(bookIndexRoot, title);
    if (!book) {
        cout << "Khong tim thay sach voi ten: " << title << "\n";
        return;
    }
    cout << "Tim thay sach: \n";
    cout << "ID: " << book->id << ", Ten: " << book->title << ", Tac gia: " << book->author
         << ", Nam: " << book->year << ", Trang thai: " << (book->borrowed ? "Da muon" : "Con sach") << "\n";
}

void addMember(int id, const string &name) {
    if (findMember(id)) {
        cout << "Ma thanh vien da ton tai.\n";
        return;
    }
    Member member = {id, name, 0};
    MemberNode *node = new MemberNode{member, nullptr};
    if (!memberHead) {
        memberHead = node;
    } else {
        MemberNode *current = memberHead;
        while (current->next) current = current->next;
        current->next = node;
    }
    pushOperation(Operation{Operation::ADD_MEMBER, 0, id});
    cout << "Da them thanh vien.\n";
}

void displayMembers() {
    if (!memberHead) {
        cout << "Chua co thanh vien.\n";
        return;
    }
    cout << "\nDanh sach thanh vien:\n";
    cout << "ID\tTen\tSo sach da muon\n";
    MemberNode *current = memberHead;
    while (current) {
        cout << current->data.id << "\t" << current->data.name << "\t" << current->data.borrowedCount << "\n";
        current = current->next;
    }
}

Member *findMember(int id) {
    MemberNode *current = memberHead;
    while (current) {
        if (current->data.id == id) return &current->data;
        current = current->next;
    }
    return nullptr;
}

bool borrowBook(int memberId, int bookId) {
    Member *member = findMember(memberId);
    Book *book = findBook(bookId);
    if (!member) {
        cout << "Khong tim thay thanh vien.\n";
        return false;
    }
    if (!book) {
        cout << "Khong tim thay sach.\n";
        return false;
    }
    if (book->borrowed) {
        cout << "Sach da duoc muon.\n";
        return false;
    }
    book->borrowed = true;
    book->borrowedBy = memberId;
    member->borrowedCount += 1;
    pushOperation(Operation{Operation::BORROW_BOOK, bookId, memberId});
    cout << "Muon sach thanh cong.\n";
    return true;
}

bool returnBook(int memberId, int bookId) {
    Member *member = findMember(memberId);
    Book *book = findBook(bookId);
    if (!member) {
        cout << "Khong tim thay thanh vien.\n";
        return false;
    }
    if (!book) {
        cout << "Khong tim thay sach.\n";
        return false;
    }
    if (!book->borrowed || book->borrowedBy != memberId) {
        cout << "Sach nay khong duoc muon boi thanh vien nay.\n";
        return false;
    }
    book->borrowed = false;
    book->borrowedBy = 0;
    member->borrowedCount -= 1;
    pushOperation(Operation{Operation::RETURN_BOOK, bookId, memberId});
    cout << "Tra sach thanh cong.\n";
    return true;
}

void enqueueRequest(int memberId, int bookId) {
    Member *member = findMember(memberId);
    Book *book = findBook(bookId);
    if (!member) {
        cout << "Khong tim thay thanh vien.\n";
        return;
    }
    if (!book) {
        cout << "Khong tim thay sach.\n";
        return;
    }
    enqueueRequestNode(memberId, bookId);
    cout << "Da them yeu cau muon sach vao hang doi.\n";
}

void processNextRequest() {
    BorrowRequest *request = dequeueRequestNode();
    if (!request) {
        cout << "Khong co yeu cau nao trong hang doi.\n";
        return;
    }
    cout << "Xu ly yeu cau: Thanh vien " << request->memberId << " yeu cau muon sach " << request->bookId << "\n";
    bool success = borrowBook(request->memberId, request->bookId);
    if (success) {
        pushOperation(Operation{Operation::PROCESS_REQUEST, request->bookId, request->memberId});
    }
    delete request;
}

void undoLastOperation() {
    Operation op;
    if (!popOperation(op)) {
        cout << "Khong co hanh dong nao de hoan tac.\n";
        return;
    }
    switch (op.type) {
        case Operation::ADD_BOOK:
            removeBook(op.bookId);
            cout << "Hoan tac: da xoa sach moi them.\n";
            break;
        case Operation::REMOVE_BOOK:
            cout << "Hoan tac: khong hoan tac duoc xoa sach do khong luu thong tin cu.\n";
            break;
        case Operation::ADD_MEMBER:
            {
                MemberNode *current = memberHead;
                MemberNode *previous = nullptr;
                while (current && current->data.id != op.memberId) {
                    previous = current;
                    current = current->next;
                }
                if (current) {
                    if (!previous) memberHead = current->next;
                    else previous->next = current->next;
                    delete current;
                    cout << "Hoan tac: da xoa thanh vien moi them.\n";
                }
            }
            break;
        case Operation::BORROW_BOOK:
            {
                Book *book = findBook(op.bookId);
                Member *member = findMember(op.memberId);
                if (book && member && book->borrowed && book->borrowedBy == op.memberId) {
                    book->borrowed = false;
                    book->borrowedBy = 0;
                    member->borrowedCount -= 1;
                    cout << "Hoan tac: da tra lai sach.\n";
                } else {
                    cout << "Hoan tac: khong the hoan tac muon sach.\n";
                }
            }
            break;
        case Operation::RETURN_BOOK:
            {
                Book *book = findBook(op.bookId);
                Member *member = findMember(op.memberId);
                if (book && member && !book->borrowed) {
                    book->borrowed = true;
                    book->borrowedBy = op.memberId;
                    member->borrowedCount += 1;
                    cout << "Hoan tac: da muon lai sach.\n";
                } else {
                    cout << "Hoan tac: khong the hoan tac tra sach.\n";
                }
            }
            break;
        case Operation::PROCESS_REQUEST:
            cout << "Hoan tac: khong hoan tac duoc yeu cau da xu ly.\n";
            break;
        default:
            cout << "Loai hoan tac khong duoc ho tro.\n";
            break;
    }
}
