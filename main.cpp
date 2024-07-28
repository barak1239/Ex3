#include "catan.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(nullptr)); // generates a seed it stays random because time changes. seed determines the outcome of random functions
    std::cout << "Welcome to Catan!" << std::endl;

    // Initialize the game with 3 players
    Catan game(3);
    game.initializeGame();

    std::cout << "Game initialized. Starting one round." << std::endl;

    int i=1;
    // Play one round (each player takes a turn)
    while(game.getGameState()!= GameState::GameOver) {
        std::cout << "\n--- Turn " << i << " ---" << std::endl;
        game.playTurn();

        if (game.getGameState() == GameState::GameOver) {
            std::cout << "Game over! " << game.getCurrentPlayer().getName() << " wins!" << std::endl;
            break;
        }
        i++;
    }

    std::cout << "\nRound complete." << std::endl;

    return 0;
}