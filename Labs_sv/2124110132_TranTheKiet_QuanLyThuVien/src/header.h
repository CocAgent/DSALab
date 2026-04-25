#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct Book {
    int id;
    string title;
    string author;
    int year;
    bool borrowed;
    int borrowedBy;
};

struct Member {
    int id;
    string name;
    int borrowedCount;
};

struct BookNode {
    Book data;
    BookNode *next;
};

struct MemberNode {
    Member data;
    MemberNode *next;
};

struct Operation {
    enum Type {
        ADD_BOOK,
        REMOVE_BOOK,
        ADD_MEMBER,
        BORROW_BOOK,
        RETURN_BOOK,
        PROCESS_REQUEST
    } type;
    int bookId;
    int memberId;
};

struct OpNode {
    Operation data;
    OpNode *next;
};

struct BorrowRequest {
    int memberId;
    int bookId;
    BorrowRequest *next;
};

struct BSTNode {
    Book *book;
    BSTNode *left;
    BSTNode *right;
};

void initializeLibrary();
void addBook(int id, const string &title, const string &author, int year);
void removeBook(int id);
void updateBook(int id, const string &newTitle, const string &newAuthor, int newYear);
void displayBooks();
Book *findBook(int id);
void searchBookByTitle(const string &title);

void addMember(int id, const string &name);
void displayMembers();
Member *findMember(int id);

bool borrowBook(int memberId, int bookId);
bool returnBook(int memberId, int bookId);

void enqueueRequest(int memberId, int bookId);
void processNextRequest();

void undoLastOperation();

void showMenu();
void showBookMenu();
void showMemberMenu();
void showLoanMenu();

#endif // HEADER_H
