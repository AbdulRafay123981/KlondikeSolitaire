#include "GameBoard.h"

#include <iostream>

int main() {
  GameBoard board;
  board.initialize();

  char command;

  while (true) {
    board.display();

    std::cout << "\nCommands:\n";
    std::cout << "D = Draw from stock\n";
    std::cout << "R = Recycle waste\n";
    std::cout << "M = Move tableau sequence\n";
    std::cout << "Q = Quit\n";
    std::cout << "Enter command: ";

    std::cin >> command;

    if (command == 'D' || command == 'd') {
      if (!board.drawFromStock()) {
        std::cout << "\nStock is empty. Recycle the waste first.\n";
      }
    } else if (command == 'R' || command == 'r') {
      if (!board.recycleWaste()) {
        std::cout << "\nCannot recycle right now.\n";
      } else {
        std::cout << "\nWaste recycled into stock.\n";
      }
    } else if (command == 'M' || command == 'm') {
      int source;
      int position;
      int destination;

      std::cout << "Source column (1-7): ";
      std::cin >> source;

      std::cout << "Starting card position (1 = top): ";
      std::cin >> position;

      std::cout << "Destination column (1-7): ";
      std::cin >> destination;

      if (board.moveTableauSequence(source - 1, position - 1,
                                    destination - 1)) {
        std::cout << "\nMove successful!\n";
      } else {
        std::cout << "\nInvalid tableau move.\n";
      }
    } else if (command == 'Q' || command == 'q') {
      std::cout << "\nThanks for playing!\n";
      break;
    } else {
      std::cout << "\nInvalid command.\n";
    }
  }

  return 0;
}
