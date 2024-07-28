#include "card.hpp"
#include "player.hpp"
#include "catan.hpp"
#include <algorithm>
#include <stdexcept>

DevelopmentCard::DevelopmentCard(std::string n, std::string d) : name(n), description(d) {}

std::string DevelopmentCard::getName() const { return name; }
std::string DevelopmentCard::getDescription() const { return description; }

KnightCard::KnightCard() : DevelopmentCard("Knight", "Counts towards Largest Army.") {}

void KnightCard::play(Player& player, Catan& game) {
    player.addKnight();
    game.checkLargestArmy(player);
}

VictoryPointCard::VictoryPointCard(std::string n) : DevelopmentCard(n, "1 Victory Point!") {}

void VictoryPointCard::play(Player& player, Catan& game) {
    player.addScore(1);
}

RoadBuildingCard::RoadBuildingCard() : DevelopmentCard("Road Building", "Build two roads for free.") {}

void RoadBuildingCard::play(Player& player, Catan& game) {
    for (int i = 0; i < 2; ++i) {
        game.buildRoad(player, true);
    }
}

YearOfPlentyCard::YearOfPlentyCard() : DevelopmentCard("Year of Plenty", "Take any two resources from the bank.") {}

void YearOfPlentyCard::play(Player& player, Catan& game) {
    game.yearOfPlentyAction(player);
}

MonopolyCard::MonopolyCard() : DevelopmentCard("Monopoly", "Take all of one type of resource from all other players.") {}

void MonopolyCard::play(Player& player, Catan& game) {
    game.monopolyAction(player);
}

DevelopmentDeck::DevelopmentDeck() : rng(std::random_device{}()) {
    for (int i = 0; i < 14; ++i) {
        cards.push_back(std::make_shared<KnightCard>());
    }
    cards.push_back(std::make_shared<VictoryPointCard>("Victory card 1"));
    cards.push_back(std::make_shared<VictoryPointCard>("Victory card 2"));
    cards.push_back(std::make_shared<VictoryPointCard>("Victory card 3"));
    cards.push_back(std::make_shared<VictoryPointCard>("Victory card 4"));
    cards.push_back(std::make_shared<RoadBuildingCard>());
    cards.push_back(std::make_shared<RoadBuildingCard>());
    cards.push_back(std::make_shared<YearOfPlentyCard>());
    cards.push_back(std::make_shared<YearOfPlentyCard>());
    cards.push_back(std::make_shared<MonopolyCard>());
    cards.push_back(std::make_shared<MonopolyCard>());

    std::shuffle(cards.begin(), cards.end(), rng);
}

std::shared_ptr<DevelopmentCard> DevelopmentDeck::drawCard() {
    if (cards.empty()) {
        throw std::runtime_error("Development card deck is empty!");
    }
    auto card = cards.back();
    cards.pop_back();
    return card;
}

bool DevelopmentDeck::isEmpty() const {
    return cards.empty();
}

int DevelopmentDeck::remainingCards() const {
    return cards.size();
}
