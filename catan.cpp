#include "catan.hpp"
#include <random>
#include <iostream>
#include <algorithm>

Catan::Catan(int numPlayers) : board(), devDeck(), currentPlayerIndex(0), gameState(GameState::Setup), largestArmyOwner(nullptr), largestArmyCount(0) {
    for (int i = 0; i < numPlayers; ++i) {
        players.emplace_back("Player " + std::to_string(i + 1));
    }
}

void Catan::initializeGame() {
    board.printBoard();
    // Initial placement of settlements and roads
    for (int i = 0; i < 2; ++i) {
        for (Player& player : players) {
            placeInitialSettlement(player);
            placeInitialRoad(player);
        }
        std::reverse(players.begin(), players.end());
    }
    for(Player& player : players) {
        player.addScore(2);
    }
    gameState = GameState::MainGame;
}

void Catan::placeInitialSettlement(Player& player) {
    int hexIndex;
    int cornerIndex;

    std::cout << player.getName() << " Choose A Hex For A Settlement (1-19)\n";
    std::cin >> hexIndex;

    std::cout << player.getName() << " Choose A Corner index (1-6) For A Settlement\n";
    std::cin >> cornerIndex;

    while (!board.placeSettlement(hexIndex % 19, cornerIndex % 6, player.getName())) {
        std::cout << "That did not work, lets try again!\n";
        std::cout << player.getName() << " Choose A Hex For A Settlement (1-19)\n";
        std::cin >> hexIndex;

        std::cout << player.getName() << " Choose A Hex Corner index (1-6) For A Settlement\n";
        std::cin >> cornerIndex;
    }

    player.buildSettlement(hexIndex, cornerIndex);

    // Give initial resources for second settlement
    if (gameState == GameState::Setup) {
        for (int i = 0; i < 6; ++i) {
            int adjacentHex = (hexIndex + i) % 19;
            ResourceType resource = board.getHexResource(adjacentHex);
            if (resource != ResourceType::Desert) {
                player.addResource(resource, 1);
            }
        }
    }
}

void Catan::placeInitialRoad(Player& player) {
    int startCorner;
    int endCorner;

    std::cout << player.getName() << " Choose A Starting Corner For A Road (1-54)\n";
    std::cin >> startCorner;

    std::cout << player.getName() << " Choose An Ending Corner For A Road (1-54)\n";
    std::cin >> endCorner;

    while (!board.placeRoad(startCorner, endCorner, player.getName())) {
        std::cout << "That did not work, lets try again!\n";
        std::cout << player.getName() << " Choose A Starting Corner For A Road (1-54)\n";
        std::cin >> startCorner;

        std::cout << player.getName() << " Choose An Ending Corner For A Road (1-54)\n";
        std::cin >> endCorner;
    }
    player.buildRoad(startCorner, endCorner);
}

void Catan::playTurn() {
    for(const auto& player : players) {
        std::cout << player.getName() << " has " << player.getScore() << " points\n";
    }
    Player& currentPlayer = players[currentPlayerIndex];
    std::cout << "It's " << currentPlayer.getName() << "'s turn." << std::endl;

    rollDice();

    // Player actions
    bool endTurnFlag = false;
    while (!endTurnFlag) {
        std::cout << "Choose an action:\n1. Build\n2. Trade\n3. Play development card\n4. End turn\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            build(currentPlayer);
            break;
        case 2:
            trade(currentPlayer);
            break;
        case 3:
            playDevelopmentCard(currentPlayer);
            break;
        case 4:
            endTurnFlag = true;
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    if (checkVictory()) {
        std::cout << currentPlayer.getName() << " wins the game!" << std::endl;
    }
    else {
        endTurn();
    }
}

void Catan::rollDice() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);
    int roll = dis(gen) + dis(gen);
    std::cout << "Dice roll: " << roll << std::endl;

    distributeResources(roll);
}

void Catan::distributeResources(int diceRoll) {
    for (const auto& hex : board.getHexes()) {
        if (hex.diceValue == diceRoll && hex.type != ResourceType::Desert) {
            for (const auto& settlement : board.getSettlements()) {
                const auto& pos = settlement.first;
                const auto& playerName = settlement.second;
                if (pos.first == hex.index) {
                    auto it = std::find_if(players.begin(), players.end(),
                        [&playerName](const Player& p) { return p.getName() == playerName; });
                    if (it != players.end()) {
                        it->addResource(hex.type, 1);
                    }
                }
            }
        }
    }
}

bool Catan::checkVictory() {
    for (const auto& player : players) {
        if (player.getScore() >= 10) {
            gameState = GameState::GameOver;
            return true;
        }
    }
    return false;
}

void Catan::endTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

const Player& Catan::getCurrentPlayer() const {
    return players[currentPlayerIndex];
}

GameState Catan::getGameState() const {
    return gameState;
}

void Catan::build(Player& player) {
    std::cout << "What would you like to build?\n1. Settlement\n2. Road\n3. Buy development card\n";
    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        if (player.canBuildSettlement()) {
            int hexIndex, cornerIndex;
            std::cout << "Enter hex index and corner index: ";
            std::cin >> hexIndex >> cornerIndex;
            if (board.placeSettlement(hexIndex, cornerIndex, player.getName())) {
                player.buildSettlement(hexIndex, cornerIndex);
                std::cout << "Settlement built successfully.\n";
            }
            else {
                std::cout << "Invalid placement. Try again.\n";
            }
        }
        else {
            std::cout << "Not enough resources to build a settlement.\n";
        }
        break;
    case 2:
        buildRoad(player);
        break;
    case 3:
        if (player.canBuyDevCard()) {
            if (!devDeck.isEmpty()) {
                player.buyDevCard(devDeck.drawCard());
                std::cout << "Development card purchased.\n";
            }
            else {
                std::cout << "No more development cards available.\n";
            }
        }
        else {
            std::cout << "Not enough resources to buy a development card.\n";
        }
        break;
    default:
        std::cout << "Invalid choice.\n";
    }
}

void Catan::buildRoad(Player& player, bool free) {
    if (free || player.canBuildRoad()) {
        int startCorner, endCorner;
        std::cout << "Enter start and end corner indices: ";
        std::cin >> startCorner >> endCorner;
        if (board.placeRoad(startCorner, endCorner, player.getName())) {
            if (!free) {
                player.buildRoad(startCorner, endCorner);
            }
            std::cout << "Road built successfully.\n";
        }
        else {
            std::cout << "Invalid placement. Try again.\n";
        }
    }
    else {
        std::cout << "Not enough resources to build a road.\n";
    }
}

void Catan::trade(Player& player) {
    std::cout << player.getName() << ", you can trade with the bank at a 4:1 ratio.\n";
    std::cout << "Your current resources:\n";
    for (const auto& resource : { ResourceType::Wood, ResourceType::Brick, ResourceType::Ore, ResourceType::Grain, ResourceType::Wool }) {
        std::cout << getResourceName(resource) << ": " << player.getResourceCount(resource) << "\n";
    }

    std::cout << "Enter the resource you want to trade (1-5) or -1 to cancel:\n";
    std::cout << "1: Wood, 2: Brick, 3: Ore, 4: Grain, 5: Wool\n";
    int giveResource;
    std::cin >> giveResource;

    if (giveResource == -1) {
        std::cout << "Trade cancelled.\n";
        return;
    }

    if (giveResource < 1 || giveResource > 5) {
        std::cout << "Invalid resource selection.\n";
        return;
    }

    ResourceType giveType = static_cast<ResourceType>(giveResource-1);

    if (player.getResourceCount(giveType) < 4) {
        std::cout << "You don't have enough " << getResourceName(giveType) << " to trade.\n";
        return;
    }

    std::cout << "Enter the resource you want to receive (1-5):\n";
    int receiveResource;
    std::cin >> receiveResource;

    if (receiveResource < 1 || receiveResource > 5) {
        std::cout << "Invalid resource selection.\n";
        return;
    }

    ResourceType receiveType = static_cast<ResourceType>(receiveResource-1);

    player.removeResource(giveType, 4);
    player.addResource(receiveType, 1);

    std::cout << "Trade completed. You gave 4 " << getResourceName(giveType)
        << " and received 1 " << getResourceName(receiveType) << ".\n";
}

void Catan::playDevelopmentCard(Player& player) {
    std::cout << player.getName() << ", you have the following development cards:\n";
    const auto& devCards = player.getDevCards();

    if (devCards.empty()) {
        std::cout << "You have no development cards to play.\n";
        return;
    }

    for (size_t i = 0; i < devCards.size(); ++i) {
        std::cout << i << ": " << devCards[i]->getName() << "\n";
    }

    std::cout << "Enter the number of the card you want to play or -1 to cancel:\n";
    int cardIndex;
    std::cin >> cardIndex;

    if (cardIndex == -1) {
        std::cout << "Cancelled playing development card.\n";
        return;
    }

    if (cardIndex < 0 || static_cast<size_t>(cardIndex) >= devCards.size()) {
        std::cout << "Invalid card selection.\n";
        return;
    }

    player.playDevCard(cardIndex, *this);
}

void Catan::yearOfPlentyAction(Player& player) {
    for (int i = 0; i < 2; ++i) {
        ResourceType resource = static_cast<ResourceType>(rand() % 5);  // Excluding Desert
        player.addResource(resource, 1);
        std::cout << player.getName() << " received 1 " << getResourceName(resource) << std::endl;
    }
}

void Catan::monopolyAction(Player& player) {
    ResourceType resource = static_cast<ResourceType>(rand() % 5);  // Excluding Desert
    int totalStolen = 0;
    for (auto& otherPlayer : players) {
        if (&otherPlayer != &player) {
            int amount = otherPlayer.getResourceCount(resource);
            otherPlayer.removeResource(resource, amount);
            totalStolen += amount;
        }
    }
    player.addResource(resource, totalStolen);
    std::cout << player.getName() << " stole " << totalStolen << " " << getResourceName(resource) << " from other players." << std::endl;
}

void Catan::checkLargestArmy(Player& player) {
    if (player.getKnightCount() >= 3 && player.getKnightCount() > largestArmyCount) {
        if (largestArmyOwner != nullptr) {
            largestArmyOwner->addScore(-2);
        }
        largestArmyOwner = &player;
        largestArmyCount = player.getKnightCount();
        player.addScore(2);
        std::cout << player.getName() << " now has the Largest Army!" << std::endl;
    }
}

std::string Catan::getResourceName(ResourceType type) {
    switch (type) {
    case ResourceType::Wood: return "Wood";
    case ResourceType::Brick: return "Brick";
    case ResourceType::Ore: return "Ore";
    case ResourceType::Grain: return "Grain";
    case ResourceType::Wool: return "Wool";
    default: return "Unknown";
    }
}
