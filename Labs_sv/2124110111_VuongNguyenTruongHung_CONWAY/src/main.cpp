#include "structures.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::cout << prompt;
        int value = 0;
        if (std::cin >> value && value >= minValue && value <= maxValue) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Input khong hop le. Vui long nhap lai.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::size_t readTopN() {
    return static_cast<std::size_t>(readInt("Nhap N (1-20): ", 1, 20));
}

void printItem(const Item& item) {
    std::cout << "- [" << item.id << "] " << item.name << " | "
              << item.category << " | views=" << item.totalViews
              << " | likes=" << item.totalLikes
              << " | avgRating=" << std::fixed << std::setprecision(2)
              << item.averageRating() << '\n';
}

void printRecommendationList(const std::vector<std::pair<Item, double>>& recommendations) {
    if (recommendations.empty()) {
        std::cout << "Khong co goi y phu hop.\n";
        return;
    }

    int rank = 1;
    for (const auto& entry : recommendations) {
        std::cout << rank++ << ". [" << entry.first.id << "] " << entry.first.name
                  << " | score=" << std::fixed << std::setprecision(3) << entry.second
                  << " | category=" << entry.first.category << '\n';
    }
}

void showMenu() {
    std::cout << "\n===== RECOMMENDATION ENGINE =====\n";
    std::cout << "1. Them nguoi dung\n";
    std::cout << "2. Them item vao catalog\n";
    std::cout << "3. Ghi nhan hanh dong (view / like / rating)\n";
    std::cout << "4. Tim item trong BST theo ID\n";
    std::cout << "5. Hien thi catalog\n";
    std::cout << "6. Goi y theo nguoi dung tuong tu\n";
    std::cout << "7. Goi y theo item tuong tu\n";
    std::cout << "8. Hien thi Top-N goi y tong hop\n";
    std::cout << "9. Bao cao thong ke\n";
    std::cout << "10. Xem lich su nguoi dung\n";
    std::cout << "11. Nap du lieu mau de demo\n";
    std::cout << "0. Thoat\n";
}
}  // namespace

int main() {
    RecommendationEngine engine;

    while (true) {
        showMenu();
        const int choice = readInt("Chon chuc nang: ", 0, 11);

        if (choice == 0) {
            std::cout << "Tam biet.\n";
            break;
        }

        if (choice == 1) {
            const std::string userId = readLine("Nhap user ID: ");
            const std::string name = readLine("Nhap ten nguoi dung: ");
            if (engine.addUser(userId, name)) {
                std::cout << "Da them nguoi dung.\n";
            } else {
                std::cout << "Khong the them nguoi dung. Kiem tra ID co bi trung hoac rong khong.\n";
            }
            continue;
        }

        if (choice == 2) {
            Item item;
            item.id = readLine("Nhap item ID: ");
            item.name = readLine("Nhap ten item: ");
            item.category = readLine("Nhap category: ");
            item.description = readLine("Nhap mo ta ngan: ");
            if (engine.addItem(item)) {
                std::cout << "Da them item vao catalog.\n";
            } else {
                std::cout << "Khong the them item. Kiem tra ID co bi trung hoac rong khong.\n";
            }
            continue;
        }

        if (choice == 3) {
            const std::string userId = readLine("Nhap user ID: ");
            const std::string itemId = readLine("Nhap item ID: ");
            const int action = readInt("Chon hanh dong (1=view, 2=like, 3=rating): ", 1, 3);

            bool success = false;
            if (action == 1) {
                success = engine.recordView(userId, itemId);
            } else if (action == 2) {
                success = engine.recordLike(userId, itemId);
            } else {
                const int rating = readInt("Nhap rating (1-5): ", 1, 5);
                success = engine.recordRating(userId, itemId, rating);
            }

            std::cout << (success ? "Da ghi nhan hanh dong.\n" : "Ghi nhan that bai. Kiem tra user/item/rating.\n");
            continue;
        }

        if (choice == 4) {
            const std::string itemId = readLine("Nhap item ID can tim: ");
            const Item* item = engine.findItemConst(itemId);
            if (item == nullptr) {
                std::cout << "Khong tim thay item.\n";
            } else {
                printItem(*item);
                std::cout << "  Mo ta: " << item->description << '\n';
            }
            continue;
        }

        if (choice == 5) {
            const std::vector<Item> catalog = engine.listCatalog();
            if (catalog.empty()) {
                std::cout << "Catalog dang rong.\n";
            } else {
                for (const Item& item : catalog) {
                    printItem(item);
                }
            }
            continue;
        }

        if (choice == 6) {
            const std::string userId = readLine("Nhap user ID: ");
            const std::size_t topN = readTopN();
            printRecommendationList(engine.recommendBySimilarUsers(userId, topN));
            continue;
        }

        if (choice == 7) {
            const std::string userId = readLine("Nhap user ID: ");
            const std::size_t topN = readTopN();
            printRecommendationList(engine.recommendBySimilarItems(userId, topN));
            continue;
        }

        if (choice == 8) {
            const std::string userId = readLine("Nhap user ID: ");
            const std::size_t topN = readTopN();
            printRecommendationList(engine.recommendTopN(userId, topN));
            continue;
        }

        if (choice == 9) {
            const Item popularItem = engine.reportMostPopularItem();
            const User activeUser = engine.reportMostActiveUser();

            if (popularItem.id.empty()) {
                std::cout << "Chua co du lieu de thong ke.\n";
            } else {
                std::cout << "Item pho bien nhat:\n";
                printItem(popularItem);
            }

            if (!activeUser.id.empty()) {
                std::cout << "Nguoi dung hoat dong nhieu nhat: [" << activeUser.id
                          << "] " << activeUser.name << '\n';
            }
            continue;
        }

        if (choice == 10) {
            const std::string userId = readLine("Nhap user ID: ");
            const auto history = engine.getUserHistory(userId);
            if (history.empty()) {
                std::cout << "Nguoi dung chua co lich su hoac khong ton tai.\n";
            } else {
                for (const auto& entry : history) {
                    std::cout << "- Item " << entry.first
                              << " | views=" << entry.second.views
                              << " | liked=" << (entry.second.liked ? "yes" : "no")
                              << " | avgRating=" << std::fixed << std::setprecision(2)
                              << entry.second.averageRating() << '\n';
                }
            }
            continue;
        }

        if (choice == 11) {
            engine.loadSampleData();
            std::cout << "Da nap du lieu mau.\n";
        }
    }

    return 0;
}
