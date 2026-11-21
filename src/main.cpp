#include "GameBoard.h"
#include "GameHistory.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

bool readNumber(int& number) {
    if (std::cin >> number) {
        return true;
    }

    if (std::cin.eof()) {
        return false;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

bool readCommand(char& command) {
    if (std::cin >> command) {
        return true;
    }

    return false;
}

int main() {
    GameBoard board;
    GameHistory history;

    board.initialize();

    char command;
    int drawCount = 1;
    std::string message = "Welcome to Klondike Solitaire!";

    while (true) {
        clearScreen();
        board.display();

        std::cout << "\nDraw mode: " << drawCount << "\n";
        std::cout << "\n" << message << "\n";

        std::cout << "\nCommands:\n";
        std::cout << "1 = Draw-one mode\n";
        std::cout << "3 = Draw-three mode\n";
        std::cout << "D = Draw from stock\n";
        std::cout << "R = Recycle waste\n";
        std::cout << "M = Move tableau sequence\n";
        std::cout << "A = Move waste to tableau\n";
        std::cout << "F = Move waste to foundation\n";
        std::cout << "G = Move tableau to foundation\n";
        std::cout << "U = Undo last move\n";
        std::cout << "Y = Redo undone move\n";
        std::cout << "Q = Quit\n";
        std::cout << "Enter command: ";

        if (!readCommand(command)) {
            break;
        }

        if (command == '1') {
            drawCount = 1;
            message = "Draw-one mode selected.";

        } else if (command == '3') {
            drawCount = 3;
            message = "Draw-three mode selected.";

        } else if (command == 'D' || command == 'd') {
            GameBoard previousState = board;

            if (board.drawFromStock(drawCount)) {
                history.record(previousState);
                message = "Cards drawn successfully.";
            } else {
                message = "Stock is empty. Recycle the waste first.";
            }

        } else if (command == 'R' || command == 'r') {
            GameBoard previousState = board;

            if (board.recycleWaste()) {
                history.record(previousState);
                message = "Waste recycled into stock.";
            } else {
                message = "Cannot recycle right now.";
            }

        } else if (command == 'M' || command == 'm') {
            int source, position, destination;

            std::cout << "Source column (1-7): ";
            if (!readNumber(source)) {
                message = "Invalid input! Please enter a number.";
                continue;
            }

            std::cout << "Starting card position (1 = top): ";
            if (!readNumber(position)) {
                message = "Invalid input! Please enter a number.";
                continue;
            }

            std::cout << "Destination column (1-7): ";
            if (!readNumber(destination)) {
                message = "Invalid input! Please enter a number.";
                continue;
            }

            GameBoard previousState = board;

            if (board.moveTableauSequence(source - 1, position - 1,
                                          destination - 1)) {
                history.record(previousState);
                message = "Move successful!";
            } else {
                message = "Invalid tableau move.";
            }

        } else if (command == 'A' || command == 'a') {
            int destination;

            std::cout << "Destination column (1-7): ";
            if (!readNumber(destination)) {
                message = "Invalid input! Please enter a number.";
                continue;
            }

            GameBoard previousState = board;

            if (board.moveWasteToTableau(destination - 1)) {
                history.record(previousState);
                message = "Waste card moved to tableau!";
            } else {
                message = "Invalid waste-to-tableau move.";
            }

        } else if (command == 'F' || command == 'f') {
            GameBoard previousState = board;

            if (board.moveWasteToFoundation()) {
                history.record(previousState);
                message = "Waste card moved to foundation!";
            } else {
                message = "Invalid waste-to-foundation move.";
            }

        } else if (command == 'G' || command == 'g') {
            int source;

            std::cout << "Source column (1-7): ";
            if (!readNumber(source)) {
                message = "Invalid input! Please enter a number.";
                continue;
            }

            GameBoard previousState = board;

            if (board.moveTableauToFoundation(source - 1)) {
                history.record(previousState);
                message = "Tableau card moved to foundation!";
            } else {
                message = "Invalid tableau-to-foundation move.";
            }

        } else if (command == 'U' || command == 'u') {
            if (history.undo(board)) {
                message = "Last move undone.";
            } else {
                message = "Nothing to undo.";
            }

        } else if (command == 'Y' || command == 'y') {
            if (history.redo(board)) {
                message = "Move restored.";
            } else {
                message = "Nothing to redo.";
            }

        } else if (command == 'Q' || command == 'q') {
            std::cout << "\nThanks for playing!\n";
            break;

        } else {
            message = "Invalid command.";
        }

        if (board.hasWon()) {
            message = "CONGRATULATIONS! YOU WON!";
            clearScreen();
            board.display();
            std::cout << "\n" << message << "\n";
            break;
        }
    }

    return 0;
}

