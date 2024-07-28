#pragma once

#include "board.hpp"
#include "card.hpp"
#include "player.hpp"
#include <vector>
#include <string>

enum class GameState {
    Setup,
    MainGame,
    GameOver
};

class Catan {
private:
    Board board;
    DevelopmentDeck devDeck;
    std::vector<Player> players;
    int currentPlayerIndex;
    GameState gameState;
    Player* largestArmyOwner;
    int largestArmyCount;

public:
    Catan(int numPlayers);
    void initializeGame();
    void playTurn();
    void rollDice();
    void distributeResources(int diceRoll);
    bool checkVictory();
    void endTurn();
    const Player& getCurrentPlayer() const;
    GameState getGameState() const;
    void buildRoad(Player& player, bool free = false);
    void yearOfPlentyAction(Player& player);
    void monopolyAction(Player& player);
    void checkLargestArmy(Player& player);
    void placeInitialSettlement(Player& player);
    void placeInitialRoad(Player& player);
    void build(Player& player);
    void trade(Player& player);
    void playDevelopmentCard(Player& player);
    std::string getResourceName(ResourceType type);
};
