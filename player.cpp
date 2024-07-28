#include "player.hpp"
#include "catan.hpp"
#include <algorithm>
#include <random>

Player::Player(std::string n) : name(n), score(0), knightCount(0) {
    for (const auto& resource : { ResourceType::Wood, ResourceType::Brick, ResourceType::Ore, ResourceType::Grain, ResourceType::Wool }) {
        resources[resource] = 0;
    }
}

void Player::addResource(ResourceType type, int amount) {
    resources[type] += amount;
}

bool Player::removeResource(ResourceType type, int amount) {
    if (resources[type] >= amount) {
        resources[type] -= amount;
        return true;
    }
    return false;
}

bool Player::canBuildSettlement() const {
    return resources.at(ResourceType::Wood) >= 1 &&
        resources.at(ResourceType::Brick) >= 1 &&
        resources.at(ResourceType::Grain) >= 1 &&
        resources.at(ResourceType::Wool) >= 1;
}

bool Player::canBuildRoad() const {
    return resources.at(ResourceType::Wood) >= 1 &&
        resources.at(ResourceType::Brick) >= 1;
}

bool Player::canBuyDevCard() const {
    return resources.at(ResourceType::Ore) >= 1 &&
        resources.at(ResourceType::Wool) >= 1 &&
        resources.at(ResourceType::Grain) >= 1;
}

void Player::buildSettlement(int hexIndex, int cornerIndex) {
    if (canBuildSettlement()) {
        removeResource(ResourceType::Wood, 1);
        removeResource(ResourceType::Brick, 1);
        removeResource(ResourceType::Grain, 1);
        removeResource(ResourceType::Wool, 1);
        settlements.emplace_back(hexIndex, cornerIndex);
        score += 1;
    }
}


void Player::buildRoad(int startCorner, int endCorner) {
    if (canBuildRoad()) {
        removeResource(ResourceType::Wood, 1);
        removeResource(ResourceType::Brick, 1);
        roads.emplace_back(startCorner, endCorner);
    }
}

void Player::buyDevCard(std::shared_ptr<DevelopmentCard> card) {
    if (canBuyDevCard()) {
        removeResource(ResourceType::Ore, 1);
        removeResource(ResourceType::Wool, 1);
        removeResource(ResourceType::Grain, 1);
        devCards.push_back(card);
    }
}

void Player::playDevCard(int cardIndex, Catan& game) {
    if (cardIndex >= 0 && static_cast<size_t>(cardIndex) < devCards.size()) {
        devCards[cardIndex]->play(*this, game);
        devCards.erase(devCards.begin() + cardIndex);
    }
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

std::string Player::getName() const {
    return name;
}

int Player::getResourceCount(ResourceType type) const {
    return resources.at(type);
}

int Player::getTotalResources() const {
    int total = 0;
    for (const auto& [type, count] : resources) {
        total += count;
    }
    return total;
}

const std::vector<std::pair<int, int>>& Player::getSettlements() const {
    return settlements;
}


const std::vector<std::pair<int, int>>& Player::getRoads() const {
    return roads;
}

void Player::addKnight() {
    knightCount++;
}

int Player::getKnightCount() const {
    return knightCount;
}

const std::vector<std::shared_ptr<DevelopmentCard>>& Player::getDevCards() const {
    return devCards;
}