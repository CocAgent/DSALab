#include "structures.h"

#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>

namespace {
struct RankedCandidate {
    double score;
    std::string itemId;

    bool operator<(const RankedCandidate& other) const {
        return score < other.score;
    }
};
}  // namespace

double Item::averageRating() const {
    if (ratingCount == 0) {
        return 0.0;
    }
    return static_cast<double>(ratingSum) / static_cast<double>(ratingCount);
}

double Item::popularityScore() const {
    return static_cast<double>(totalViews) + (static_cast<double>(totalLikes) * 3.0) + (averageRating() * 2.0);
}

double Interaction::averageRating() const {
    if (ratings.empty()) {
        return 0.0;
    }

    int total = 0;
    for (int rating : ratings) {
        total += rating;
    }
    return static_cast<double>(total) / static_cast<double>(ratings.size());
}

double Interaction::preferenceScore() const {
    return static_cast<double>(views) + (liked ? 3.0 : 0.0) + (averageRating() * 2.0);
}

BSTNode::BSTNode(const Item& value) : item(value) {}

RecommendationEngine::RecommendationEngine() : root(nullptr), itemCount(0) {}

RecommendationEngine::~RecommendationEngine() {
    destroy(root);
}

bool RecommendationEngine::addUser(const std::string& userId, const std::string& name) {
    if (userId.empty() || name.empty() || hasUser(userId)) {
        return false;
    }

    users[userId] = User{userId, name};
    userHistory[userId];
    graph[userId];
    return true;
}

bool RecommendationEngine::addItem(const Item& item) {
    if (item.id.empty() || item.name.empty() || hasItem(item.id)) {
        return false;
    }

    bool inserted = false;
    root = insertNode(root, item, inserted);
    if (inserted) {
        ++itemCount;
    }
    return inserted;
}

Item* RecommendationEngine::findItem(const std::string& itemId) {
    BSTNode* node = findNode(root, itemId);
    return node == nullptr ? nullptr : &node->item;
}

const Item* RecommendationEngine::findItemConst(const std::string& itemId) const {
    BSTNode* node = findNode(root, itemId);
    return node == nullptr ? nullptr : &node->item;
}

bool RecommendationEngine::hasUser(const std::string& userId) const {
    return users.find(userId) != users.end();
}

bool RecommendationEngine::hasItem(const std::string& itemId) const {
    return findItemConst(itemId) != nullptr;
}

bool RecommendationEngine::recordView(const std::string& userId, const std::string& itemId) {
    Item* item = findItem(itemId);
    if (!hasUser(userId) || item == nullptr) {
        return false;
    }

    Interaction& interaction = userHistory[userId][itemId];
    ++interaction.views;
    ++item->totalViews;
    attachUserToItemIndex(userId, itemId);
    rebuildGraphEdge(userId, itemId);
    return true;
}

bool RecommendationEngine::recordLike(const std::string& userId, const std::string& itemId) {
    Item* item = findItem(itemId);
    if (!hasUser(userId) || item == nullptr) {
        return false;
    }

    Interaction& interaction = userHistory[userId][itemId];
    if (!interaction.liked) {
        interaction.liked = true;
        ++item->totalLikes;
    }
    attachUserToItemIndex(userId, itemId);
    rebuildGraphEdge(userId, itemId);
    return true;
}

bool RecommendationEngine::recordRating(const std::string& userId, const std::string& itemId, int rating) {
    Item* item = findItem(itemId);
    if (!hasUser(userId) || item == nullptr || rating < 1 || rating > 5) {
        return false;
    }

    Interaction& interaction = userHistory[userId][itemId];
    interaction.ratings.push_back(rating);
    item->ratingSum += rating;
    ++item->ratingCount;
    attachUserToItemIndex(userId, itemId);
    rebuildGraphEdge(userId, itemId);
    return true;
}

std::vector<Item> RecommendationEngine::listCatalog() const {
    std::vector<Item> result;
    result.reserve(itemCount);
    inorder(root, result);
    return result;
}

std::vector<User> RecommendationEngine::listUsers() const {
    std::vector<User> result;
    result.reserve(users.size());
    for (const auto& entry : users) {
        result.push_back(entry.second);
    }
    std::sort(result.begin(), result.end(), [](const User& left, const User& right) {
        return left.id < right.id;
    });
    return result;
}

std::vector<std::pair<std::string, Interaction>> RecommendationEngine::getUserHistory(const std::string& userId) const {
    std::vector<std::pair<std::string, Interaction>> result;
    auto historyIt = userHistory.find(userId);
    if (historyIt == userHistory.end()) {
        return result;
    }

    for (const auto& entry : historyIt->second) {
        result.push_back(entry);
    }

    std::sort(result.begin(), result.end(), [](const auto& left, const auto& right) {
        return left.first < right.first;
    });
    return result;
}

std::vector<std::pair<Item, double>> RecommendationEngine::recommendBySimilarUsers(
    const std::string& userId,
    std::size_t topN) const {
    std::vector<std::pair<Item, double>> result;
    if (!hasUser(userId)) {
        return result;
    }

    std::unordered_map<std::string, double> candidateScores;
    for (const auto& entry : users) {
        const std::string& otherUserId = entry.first;
        if (otherUserId == userId) {
            continue;
        }

        double similarity = userSimilarity(userId, otherUserId);
        if (similarity <= 0.0) {
            continue;
        }

        const auto& otherHistory = userHistory.at(otherUserId);
        for (const auto& itemEntry : otherHistory) {
            const std::string& candidateItemId = itemEntry.first;
            if (userHasItem(userId, candidateItemId)) {
                continue;
            }

            candidateScores[candidateItemId] += similarity * itemEntry.second.preferenceScore();
        }
    }

    std::priority_queue<RankedCandidate> ranking;
    for (const auto& entry : candidateScores) {
        const Item* item = findItemConst(entry.first);
        if (item == nullptr) {
            continue;
        }

        double finalScore = entry.second + (item->popularityScore() * 0.05);
        ranking.push(RankedCandidate{finalScore, entry.first});
    }

    while (!ranking.empty() && result.size() < topN) {
        RankedCandidate candidate = ranking.top();
        ranking.pop();
        const Item* item = findItemConst(candidate.itemId);
        if (item != nullptr) {
            result.push_back({*item, candidate.score});
        }
    }

    return result;
}

std::vector<std::pair<Item, double>> RecommendationEngine::recommendBySimilarItems(
    const std::string& userId,
    std::size_t topN) const {
    std::vector<std::pair<Item, double>> result;
    if (!hasUser(userId)) {
        return result;
    }

    auto historyIt = userHistory.find(userId);
    if (historyIt == userHistory.end()) {
        return result;
    }

    std::unordered_map<std::string, double> candidateScores;
    const auto& currentHistory = historyIt->second;
    for (const auto& seedEntry : currentHistory) {
        const std::string& seedItemId = seedEntry.first;
        const auto seedUsersIt = itemUsers.find(seedItemId);
        if (seedUsersIt == itemUsers.end()) {
            continue;
        }

        for (const std::string& relatedUserId : seedUsersIt->second) {
            const auto& relatedHistory = userHistory.at(relatedUserId);
            for (const auto& candidateEntry : relatedHistory) {
                const std::string& candidateItemId = candidateEntry.first;
                if (candidateItemId == seedItemId || userHasItem(userId, candidateItemId)) {
                    continue;
                }

                double similarity = itemSimilarity(seedItemId, candidateItemId);
                if (similarity <= 0.0) {
                    continue;
                }

                candidateScores[candidateItemId] += seedEntry.second.preferenceScore() * similarity;
            }
        }
    }

    std::priority_queue<RankedCandidate> ranking;
    for (const auto& entry : candidateScores) {
        const Item* item = findItemConst(entry.first);
        if (item == nullptr) {
            continue;
        }
        double finalScore = entry.second + (item->averageRating() * 0.1);
        ranking.push(RankedCandidate{finalScore, entry.first});
    }

    while (!ranking.empty() && result.size() < topN) {
        RankedCandidate candidate = ranking.top();
        ranking.pop();
        const Item* item = findItemConst(candidate.itemId);
        if (item != nullptr) {
            result.push_back({*item, candidate.score});
        }
    }

    return result;
}

std::vector<std::pair<Item, double>> RecommendationEngine::recommendTopN(
    const std::string& userId,
    std::size_t topN) const {
    std::vector<std::pair<Item, double>> result;
    if (!hasUser(userId)) {
        return result;
    }

    std::unordered_map<std::string, double> combinedScores;
    const auto userBased = recommendBySimilarUsers(userId, itemCount);
    const auto itemBased = recommendBySimilarItems(userId, itemCount);

    for (const auto& entry : userBased) {
        combinedScores[entry.first.id] += entry.second * 0.6;
    }
    for (const auto& entry : itemBased) {
        combinedScores[entry.first.id] += entry.second * 0.4;
    }

    std::priority_queue<RankedCandidate> ranking;
    for (const auto& entry : combinedScores) {
        const Item* item = findItemConst(entry.first);
        if (item == nullptr) {
            continue;
        }
        ranking.push(RankedCandidate{entry.second + (item->popularityScore() * 0.03), entry.first});
    }

    while (!ranking.empty() && result.size() < topN) {
        RankedCandidate candidate = ranking.top();
        ranking.pop();
        const Item* item = findItemConst(candidate.itemId);
        if (item != nullptr) {
            result.push_back({*item, candidate.score});
        }
    }

    return result;
}

Item RecommendationEngine::reportMostPopularItem() const {
    std::vector<Item> items = listCatalog();
    if (items.empty()) {
        return Item{};
    }

    return *std::max_element(items.begin(), items.end(), [](const Item& left, const Item& right) {
        return left.popularityScore() < right.popularityScore();
    });
}

User RecommendationEngine::reportMostActiveUser() const {
    if (users.empty()) {
        return User{};
    }

    auto best = std::max_element(users.begin(), users.end(), [this](const auto& left, const auto& right) {
        return userActivityScore(left.first) < userActivityScore(right.first);
    });
    return best->second;
}

void RecommendationEngine::loadSampleData() {
    clear();

    addUser("U01", "An");
    addUser("U02", "Binh");
    addUser("U03", "Chi");
    addUser("U04", "Dung");

    addItem({"M01", "Interstellar", "Movie", "Sci-fi du hanh khong gian"});
    addItem({"M02", "Inception", "Movie", "Giac mo va thao tung tam tri"});
    addItem({"M03", "The Dark Knight", "Movie", "Batman doi dau Joker"});
    addItem({"M04", "Coco", "Movie", "Animated am nhac gia dinh"});
    addItem({"M05", "La La Land", "Movie", "Nhac kich lang man"});
    addItem({"M06", "Soul", "Movie", "Hanh trinh tim y nghia song"});

    recordView("U01", "M01");
    recordLike("U01", "M01");
    recordRating("U01", "M01", 5);
    recordView("U01", "M02");
    recordLike("U01", "M02");
    recordRating("U01", "M02", 4);

    recordView("U02", "M01");
    recordLike("U02", "M01");
    recordRating("U02", "M01", 5);
    recordView("U02", "M03");
    recordLike("U02", "M03");
    recordRating("U02", "M03", 5);
    recordView("U02", "M06");
    recordRating("U02", "M06", 4);

    recordView("U03", "M02");
    recordLike("U03", "M02");
    recordRating("U03", "M02", 4);
    recordView("U03", "M05");
    recordLike("U03", "M05");
    recordRating("U03", "M05", 5);
    recordView("U03", "M04");
    recordRating("U03", "M04", 4);

    recordView("U04", "M01");
    recordRating("U04", "M01", 4);
    recordView("U04", "M02");
    recordLike("U04", "M02");
    recordRating("U04", "M02", 5);
    recordView("U04", "M05");
    recordRating("U04", "M05", 5);
}

void RecommendationEngine::clear() {
    destroy(root);
    root = nullptr;
    itemCount = 0;
    users.clear();
    userHistory.clear();
    graph.clear();
    itemUsers.clear();
}

BSTNode* RecommendationEngine::insertNode(BSTNode* node, const Item& item, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new BSTNode(item);
    }

    if (item.id < node->item.id) {
        node->left = insertNode(node->left, item, inserted);
    } else if (item.id > node->item.id) {
        node->right = insertNode(node->right, item, inserted);
    } else {
        inserted = false;
    }
    return node;
}

BSTNode* RecommendationEngine::findNode(BSTNode* node, const std::string& itemId) const {
    BSTNode* current = node;
    while (current != nullptr) {
        if (itemId < current->item.id) {
            current = current->left;
        } else if (itemId > current->item.id) {
            current = current->right;
        } else {
            return current;
        }
    }
    return nullptr;
}

void RecommendationEngine::inorder(BSTNode* node, std::vector<Item>& result) const {
    if (node == nullptr) {
        return;
    }
    inorder(node->left, result);
    result.push_back(node->item);
    inorder(node->right, result);
}

void RecommendationEngine::destroy(BSTNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void RecommendationEngine::rebuildGraphEdge(const std::string& userId, const std::string& itemId) {
    const Interaction& interaction = userHistory[userId][itemId];
    const double weight = interaction.preferenceScore();
    std::vector<UserItemEdge>& edges = graph[userId];

    for (UserItemEdge& edge : edges) {
        if (edge.itemId == itemId) {
            edge.weight = weight;
            return;
        }
    }

    edges.push_back(UserItemEdge{itemId, weight});
}

void RecommendationEngine::attachUserToItemIndex(const std::string& userId, const std::string& itemId) {
    std::vector<std::string>& bucket = itemUsers[itemId];
    if (std::find(bucket.begin(), bucket.end(), userId) == bucket.end()) {
        bucket.push_back(userId);
    }
}

bool RecommendationEngine::userHasItem(const std::string& userId, const std::string& itemId) const {
    auto historyIt = userHistory.find(userId);
    if (historyIt == userHistory.end()) {
        return false;
    }
    return historyIt->second.find(itemId) != historyIt->second.end();
}

double RecommendationEngine::userSimilarity(const std::string& userA, const std::string& userB) const {
    auto historyAIt = userHistory.find(userA);
    auto historyBIt = userHistory.find(userB);
    if (historyAIt == userHistory.end() || historyBIt == userHistory.end()) {
        return 0.0;
    }

    const auto& historyA = historyAIt->second;
    const auto& historyB = historyBIt->second;

    double dot = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (const auto& entry : historyA) {
        const double scoreA = entry.second.preferenceScore();
        normA += scoreA * scoreA;

        auto otherIt = historyB.find(entry.first);
        if (otherIt != historyB.end()) {
            dot += scoreA * otherIt->second.preferenceScore();
        }
    }

    for (const auto& entry : historyB) {
        const double scoreB = entry.second.preferenceScore();
        normB += scoreB * scoreB;
    }

    if (dot == 0.0 || normA == 0.0 || normB == 0.0) {
        return 0.0;
    }

    return dot / (std::sqrt(normA) * std::sqrt(normB));
}

double RecommendationEngine::itemSimilarity(const std::string& itemA, const std::string& itemB) const {
    auto usersAIt = itemUsers.find(itemA);
    auto usersBIt = itemUsers.find(itemB);
    if (usersAIt == itemUsers.end() || usersBIt == itemUsers.end()) {
        return 0.0;
    }

    const auto& usersA = usersAIt->second;
    const auto& usersB = usersBIt->second;
    if (usersA.empty() || usersB.empty()) {
        return 0.0;
    }

    std::unordered_set<std::string> smaller;
    const std::vector<std::string>* larger = nullptr;
    if (usersA.size() <= usersB.size()) {
        smaller.insert(usersA.begin(), usersA.end());
        larger = &usersB;
    } else {
        smaller.insert(usersB.begin(), usersB.end());
        larger = &usersA;
    }

    std::size_t sharedUsers = 0;
    for (const std::string& userId : *larger) {
        if (smaller.find(userId) != smaller.end()) {
            ++sharedUsers;
        }
    }

    if (sharedUsers == 0) {
        return 0.0;
    }

    return static_cast<double>(sharedUsers) /
           std::sqrt(static_cast<double>(usersA.size()) * static_cast<double>(usersB.size()));
}

double RecommendationEngine::userActivityScore(const std::string& userId) const {
    auto historyIt = userHistory.find(userId);
    if (historyIt == userHistory.end()) {
        return 0.0;
    }

    double total = 0.0;
    for (const auto& entry : historyIt->second) {
        total += entry.second.preferenceScore();
    }
    return total;
}
