#pragma once

#include <string>
#include <memory>
#include <vector>
#include <random>

class Player;
class Catan;

class DevelopmentCard {
protected:
    std::string name;
    std::string description;

public:
    DevelopmentCard(std::string n, std::string d);
    virtual ~DevelopmentCard() = default;
    std::string getName() const;
    std::string getDescription() const;
    virtual void play(Player& player, Catan& game) = 0;
};

class KnightCard : public DevelopmentCard {
public:
    KnightCard();
    void play(Player& player, Catan& game) override;
};

class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard(std::string n);
    void play(Player& player, Catan& game) override;
};

class RoadBuildingCard : public DevelopmentCard {
public:
    RoadBuildingCard();
    void play(Player& player, Catan& game) override;
};

class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard();
    void play(Player& player, Catan& game) override;
};

class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard();
    void play(Player& player, Catan& game) override;
};

class DevelopmentDeck {
private:
    std::vector<std::shared_ptr<DevelopmentCard>> cards;
    std::mt19937 rng;

public:
    DevelopmentDeck();
    std::shared_ptr<DevelopmentCard> drawCard();
    bool isEmpty() const;
    int remainingCards() const;
};
