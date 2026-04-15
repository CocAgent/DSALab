#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Item {
    std::string id;
    std::string name;
    std::string category;
    std::string description;
    int totalViews = 0;
    int totalLikes = 0;
    int ratingSum = 0;
    int ratingCount = 0;

    double averageRating() const;
    double popularityScore() const;
};

struct User {
    std::string id;
    std::string name;
};

struct Interaction {
    int views = 0;
    bool liked = false;
    std::vector<int> ratings;

    double averageRating() const;
    double preferenceScore() const;
};

struct UserItemEdge {
    std::string itemId;
    double weight = 0.0;
};

struct BSTNode {
    Item item;
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;

    explicit BSTNode(const Item& value);
};

class RecommendationEngine {
public:
    RecommendationEngine();
    ~RecommendationEngine();

    RecommendationEngine(const RecommendationEngine&) = delete;
    RecommendationEngine& operator=(const RecommendationEngine&) = delete;

    bool addUser(const std::string& userId, const std::string& name);
    bool addItem(const Item& item);

    Item* findItem(const std::string& itemId);
    const Item* findItemConst(const std::string& itemId) const;

    bool hasUser(const std::string& userId) const;
    bool hasItem(const std::string& itemId) const;

    bool recordView(const std::string& userId, const std::string& itemId);
    bool recordLike(const std::string& userId, const std::string& itemId);
    bool recordRating(const std::string& userId, const std::string& itemId, int rating);

    std::vector<Item> listCatalog() const;
    std::vector<User> listUsers() const;
    std::vector<std::pair<std::string, Interaction>> getUserHistory(const std::string& userId) const;

    std::vector<std::pair<Item, double>> recommendBySimilarUsers(const std::string& userId, std::size_t topN) const;
    std::vector<std::pair<Item, double>> recommendBySimilarItems(const std::string& userId, std::size_t topN) const;
    std::vector<std::pair<Item, double>> recommendTopN(const std::string& userId, std::size_t topN) const;

    Item reportMostPopularItem() const;
    User reportMostActiveUser() const;

    void loadSampleData();
    void clear();

private:
    BSTNode* root;
    std::size_t itemCount;
    std::unordered_map<std::string, User> users;
    std::unordered_map<std::string, std::unordered_map<std::string, Interaction>> userHistory;
    std::unordered_map<std::string, std::vector<UserItemEdge>> graph;
    std::unordered_map<std::string, std::vector<std::string>> itemUsers;

    BSTNode* insertNode(BSTNode* node, const Item& item, bool& inserted);
    BSTNode* findNode(BSTNode* node, const std::string& itemId) const;
    void inorder(BSTNode* node, std::vector<Item>& result) const;
    void destroy(BSTNode* node);

    void rebuildGraphEdge(const std::string& userId, const std::string& itemId);
    void attachUserToItemIndex(const std::string& userId, const std::string& itemId);
    bool userHasItem(const std::string& userId, const std::string& itemId) const;
    double userSimilarity(const std::string& userA, const std::string& userB) const;
    double itemSimilarity(const std::string& itemA, const std::string& itemB) const;
    double userActivityScore(const std::string& userId) const;
};

#endif
