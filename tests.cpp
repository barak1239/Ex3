#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "catan.hpp"
#include "player.hpp"
#include "board.hpp"
#include "card.hpp"

TEST_CASE("Player initialization") {
    Player player("Test Player");
    CHECK(player.getName() == "Test Player");
    CHECK(player.getScore() == 0);
    CHECK(player.getKnightCount() == 0);
    CHECK(player.getTotalResources() == 0);
}

TEST_CASE("Resource management") {
    Player player("Test Player");
    player.addResource(ResourceType::Wood, 2);
    player.addResource(ResourceType::Brick, 1);
    CHECK(player.getResourceCount(ResourceType::Wood) == 2);
    CHECK(player.getResourceCount(ResourceType::Brick) == 1);
    CHECK(player.getTotalResources() == 3);
    
    bool removed = player.removeResource(ResourceType::Wood, 1);
    CHECK(removed);
    CHECK(player.getResourceCount(ResourceType::Wood) == 1);
    
    removed = player.removeResource(ResourceType::Ore, 1);
    CHECK_FALSE(removed);
}

TEST_CASE("Board initialization") {
    Board board;
    const auto& hexes = board.getHexes();
    CHECK(hexes.size() == 19);
    int desertCount = 0;
    for (const auto& hex : hexes) {
        if (hex.type == ResourceType::Desert) {
            desertCount++;
        }
    }
    CHECK(desertCount == 1);
}

TEST_CASE("Development deck") {
    DevelopmentDeck deck;
    CHECK(deck.remainingCards() == 24);
    auto card = deck.drawCard();
    CHECK(card != nullptr);
    CHECK(deck.remainingCards() == 23);
}