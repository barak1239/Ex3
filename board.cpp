#include "board.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>

Board::Board() : randomGen(std::random_device{}()) {
    initializeBoard();
}

void Board::initializeBoard() {
    plots();
    plotValues();
    plotAdjacencies();
}

void Board::plots() {
    std::vector<ResourceType> resourceTypes = {
        ResourceType::Wood, ResourceType::Wood, ResourceType::Wood, ResourceType::Wood,
        ResourceType::Brick, ResourceType::Brick, ResourceType::Brick,
        ResourceType::Ore, ResourceType::Ore, ResourceType::Ore,
        ResourceType::Grain, ResourceType::Grain, ResourceType::Grain, ResourceType::Grain,
        ResourceType::Wool, ResourceType::Wool, ResourceType::Wool, ResourceType::Wool,
        ResourceType::Desert
    };
    std::shuffle(resourceTypes.begin(), resourceTypes.end(), randomGen);
    for (size_t i = 0; i < resourceTypes.size(); ++i) {
        hexes.emplace_back(resourceTypes[i], 0, i);
    }
}

void Board::plotValues() {
    std::vector<int> values = { 2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12 };
    std::shuffle(values.begin(), values.end(), randomGen);
    int hexIndex = 0;
    for (auto& hex : hexes) {
        if (hex.type != ResourceType::Desert) {
            hex.diceValue = values[hexIndex++];
        }
        else {
            hex.diceValue = 7;  // Desert
        }
    }
}

void Board::plotAdjacencies() {
    plotAdjancies.resize(54);
    for (int i = 0; i < 54; ++i) {
        if (i % 2 == 0) {
            plotAdjancies[i] = { (i + 1) % 54, (i + 2) % 54 };
        }
        else {
            plotAdjancies[i] = { (i + 1) % 54, (i + 53) % 54 };
        }
    }
}

void Board::printBoard() const {
    int rowsPrinted = 0;

    std::cout << "Catan Board:\n\n";
  
    for (int i = 0; i < 19; rowsPrinted++){
        printHexRow(i, i + 5 - abs(2 - rowsPrinted), abs(2 - rowsPrinted));
        i += 5 - abs(2 - rowsPrinted);
    }

    std::cout << "\nSettlements:\n";
    for (const auto& [pos, player] : settlements) {
        std::cout << player << " has a settlement at hex " << pos.first
            << ", vertice " << pos.second << "\n";
    }

    std::cout << "\nRoads:\n";
    for (const auto& [corners, player] : roads) {
        std::cout << player << " has a road between vertecies "
            << corners.first << " and " << corners.second << "\n";
    }
}

void Board::printHexRow(int start, int end, int indent) const {
    std::string padding(indent * 4, ' ');

    // Top of hexagons
    std::cout << padding;
    for (int i = start; i < end; ++i) {
        std::cout << " ------ ";
        if (i < end - 1) std::cout << "  ";
    }
    std::cout << "\n";

    // Dice value line
    std::cout << padding;
    for (int i = start; i < end; ++i) {
        std::cout << "|  " << std::setw(2) << (hexes[i].diceValue == 7 ? "  " : std::to_string(hexes[i].diceValue)) << "   |";
        if (i < end - 1) std::cout << " ";
    }
    std::cout << "\n";

    // Resource type line
    std::cout << padding;
    for (int i = start; i < end; ++i) {
        char symbol;
        switch (hexes[i].type) {
        case ResourceType::Wood: symbol = 'W'; break;
        case ResourceType::Brick: symbol = 'B'; break;
        case ResourceType::Ore: symbol = 'O'; break;
        case ResourceType::Grain: symbol = 'G'; break;
        case ResourceType::Wool: symbol = 'L'; break;
        case ResourceType::Desert: symbol = 'D'; break;
        default: symbol = '?';
        }
        std::cout << "|   " << symbol << "   |";
        if (i < end - 1) std::cout << " ";
    }
    std::cout << "\n";

    // Bottom of hexagons
    std::cout << padding;
    for (int i = start; i < end; ++i) {
        std::cout << " ------ ";
        if (i < end - 1) std::cout << "  ";
    }
    std::cout << "\n";
}

bool Board::placeSettlement(int hexIndex, int cornerIndex, const std::string& playerName) {
    if (hexIndex < 0 || static_cast<size_t>(hexIndex) >= hexes.size() || cornerIndex < 0 || cornerIndex > 5) {
        return false;
    }

    auto key = std::make_pair(hexIndex, cornerIndex);
    if (settlements.find(key) != settlements.end()) {
        return false;  // settlement already exists
    }

    settlements[key] = playerName;
    return true;
}

bool Board::placeRoad(int startCorner, int endCorner, const std::string& playerName) {
    if (startCorner < 0 || startCorner >= 54 || endCorner < 0 || endCorner >= 54) {
        return false;
    }

    auto it = std::find(plotAdjancies[startCorner].begin(), plotAdjancies[startCorner].end(), endCorner);
    if (it == plotAdjancies[startCorner].end()) {
        return false;  // vertices are not connected
    }

    auto key = std::make_pair(std::min(startCorner, endCorner), std::max(startCorner, endCorner));
    if (roads.find(key) != roads.end()) {
        return false;  // road already exists
    }

    roads[key] = playerName;
    return true;
}

ResourceType Board::getHexResource(int hexIndex) const {
    if (hexIndex >= 0 && static_cast<size_t>(hexIndex) < hexes.size()) {
        return hexes[hexIndex].type;
    }
    return ResourceType::Desert;  // Default return if index is out of bounds
}

const std::vector<HexTile>& Board::getHexes() const {
    return hexes;
}

const std::map<std::pair<int, int>, std::string>& Board::getSettlements() const {
    return settlements;
}