#pragma once

#include "board.hpp"
#include "card.hpp"
#include "catan.hpp"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
class Player {
private:
    std::string name;
    int score;
    int knightCount;
    std::map<ResourceType, int> resources;
    std::vector<std::shared_ptr<DevelopmentCard>> devCards;
    std::vector<std::pair<int, int>> settlements;
    std::vector<std::pair<int, int>> roads;

public:
    Player(std::string name);
    void addResource(ResourceType type, int amount);
    bool removeResource(ResourceType type, int amount);
    bool canBuildSettlement() const;
    bool canBuildRoad() const;
    bool canBuyDevCard() const;
    void buildSettlement(int hexIndex, int cornerIndex);
    void buildRoad(int startCorner, int endCorner);
    void buyDevCard(std::shared_ptr<DevelopmentCard> card);
    void playDevCard(int cardIndex, Catan& game);
    int getScore() const;
    void addScore(int points);
    std::string getName() const;
    int getResourceCount(ResourceType type) const;
    int getTotalResources() const;
    const std::vector<std::pair<int, int>>& getSettlements() const;
    const std::vector<std::pair<int, int>>& getRoads() const;
    void addKnight();
    int getKnightCount() const;
    const std::vector<std::shared_ptr<DevelopmentCard>>& getDevCards() const;
};
