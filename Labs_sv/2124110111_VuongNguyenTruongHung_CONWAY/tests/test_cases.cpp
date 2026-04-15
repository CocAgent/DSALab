#include "../src/structures.h"

#include <cassert>
#include <iostream>
#include <vector>

void test_add_data() {
    RecommendationEngine engine;
    assert(engine.addUser("U01", "Alice"));
    assert(engine.addItem({"I02", "Matrix", "Movie", "Sci-fi"}));
    assert(engine.addItem({"I01", "Coco", "Movie", "Family"}));

    const std::vector<Item> catalog = engine.listCatalog();
    assert(catalog.size() == 2);
    assert(catalog[0].id == "I01");
    assert(catalog[1].id == "I02");
}

void test_search_bst() {
    RecommendationEngine engine;
    engine.addItem({"I10", "Soul", "Movie", "Animated"});
    engine.addItem({"I05", "Inception", "Movie", "Dream"});
    engine.addItem({"I15", "Dune", "Movie", "Epic"});

    const Item* found = engine.findItemConst("I05");
    assert(found != nullptr);
    assert(found->name == "Inception");
    assert(engine.findItemConst("I99") == nullptr);
}

void test_record_interactions() {
    RecommendationEngine engine;
    engine.addUser("U01", "Alice");
    engine.addItem({"I01", "Interstellar", "Movie", "Space"});

    assert(engine.recordView("U01", "I01"));
    assert(engine.recordLike("U01", "I01"));
    assert(engine.recordRating("U01", "I01", 5));

    const auto history = engine.getUserHistory("U01");
    assert(history.size() == 1);
    assert(history[0].second.views == 1);
    assert(history[0].second.liked);
    assert(history[0].second.averageRating() == 5.0);

    const Item* item = engine.findItemConst("I01");
    assert(item != nullptr);
    assert(item->totalViews == 1);
    assert(item->totalLikes == 1);
    assert(item->ratingCount == 1);
}

void test_recommendations() {
    RecommendationEngine engine;
    engine.loadSampleData();

    const auto userBased = engine.recommendBySimilarUsers("U01", 3);
    const auto itemBased = engine.recommendBySimilarItems("U01", 3);
    const auto merged = engine.recommendTopN("U01", 3);

    assert(!userBased.empty());
    assert(!itemBased.empty());
    assert(!merged.empty());

    bool hasExpectedMovie = false;
    for (const auto& entry : merged) {
        if (entry.first.id == "M03" || entry.first.id == "M05" || entry.first.id == "M06") {
            hasExpectedMovie = true;
        }
        assert(entry.second > 0.0);
    }
    assert(hasExpectedMovie);
}

void test_edge_cases() {
    RecommendationEngine engine;
    assert(!engine.addUser("", "Invalid"));
    assert(!engine.addItem({"", "No ID", "Movie", "Invalid"}));

    engine.addUser("U01", "Alice");
    engine.addItem({"I01", "Dune", "Movie", "Epic"});

    assert(!engine.recordRating("U01", "I01", 0));
    assert(!engine.recordRating("U01", "I01", 6));
    assert(!engine.recordView("U02", "I01"));
    assert(!engine.recordLike("U01", "I99"));
}

void test_reports() {
    RecommendationEngine engine;
    engine.loadSampleData();

    const Item popular = engine.reportMostPopularItem();
    const User active = engine.reportMostActiveUser();

    assert(!popular.id.empty());
    assert(!active.id.empty());
}

int main() {
    test_add_data();
    test_search_bst();
    test_record_interactions();
    test_recommendations();
    test_edge_cases();
    test_reports();

    std::cout << "All test cases passed.\n";
    return 0;
}
