#pragma once

#include <vector>
#include <map>
#include <random>
#include <string>

enum class ResourceType { Wood, Brick, Ore, Grain, Wool, Desert };

struct HexTile {
    ResourceType type;
    int diceValue;
    int index;
    HexTile(ResourceType t, int v, int i) : type(t), diceValue(v), index(i) {}
};

class Board {
public:
    Board();
    void initializeBoard();
    void printBoard() const;
    bool placeSettlement(int hexIndex, int cornerIndex, const std::string& playerName);
    bool placeRoad(int startCorner, int endCorner, const std::string& playerName);

    ResourceType getHexResource(int hexIndex) const;
    const std::vector<HexTile>& getHexes() const;
    const std::map<std::pair<int, int>, std::string>& getSettlements() const;

private:
    std::vector<HexTile> hexes;
    std::map<std::pair<int, int>, std::string> settlements;
    std::map<std::pair<int, int>, std::string> roads;
    std::mt19937 randomGen;
    void plots();
    void plotValues();
    void plotAdjacencies();
    void printHexRow(int start, int end, int indent) const;
    std::vector<std::vector<int>> plotAdjancies;
};
