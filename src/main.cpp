#include "GameBoard.h"

#include <iostream>

int main() {
  GameBoard board;
  board.initialize();

  char command;

  while (true) {
    board.display();

    std::cout << "\nCommands: D = Draw, R = Recycle, Q = Quit\n";
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
    } else if (command == 'Q' || command == 'q') {
      std::cout << "\nThanks for playing!\n";
      break;
    } else {
      std::cout << "\nInvalid command.\n";
    }
  }

  return 0;
}
