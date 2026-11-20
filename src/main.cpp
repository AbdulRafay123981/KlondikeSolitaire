#include "GameBoard.h"
#include "GameHistory.h"

#include <iostream>

int main() {
  GameBoard board;
  GameHistory history;

  board.initialize();

  char command;
  int drawCount = 1;

  while (true) {
    board.display();

    std::cout << "\nDraw mode: " << drawCount << "\n";

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

    std::cin >> command;

    if (command == '1') {
      drawCount = 1;
      std::cout << "\nDraw-one mode selected.\n";

    } else if (command == '3') {
      drawCount = 3;
      std::cout << "\nDraw-three mode selected.\n";

    } else if (command == 'D' || command == 'd') {
      GameBoard previousState = board;

      if (board.drawFromStock(drawCount)) {
        history.record(previousState);
      } else {
        std::cout << "\nStock is empty. Recycle the waste first.\n";
      }

    } else if (command == 'R' || command == 'r') {
      GameBoard previousState = board;

      if (board.recycleWaste()) {
        history.record(previousState);
        std::cout << "\nWaste recycled into stock.\n";
      } else {
        std::cout << "\nCannot recycle right now.\n";
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

      GameBoard previousState = board;

      if (board.moveTableauSequence(source - 1, position - 1,
                                    destination - 1)) {
        history.record(previousState);
        std::cout << "\nMove successful!\n";
      } else {
        std::cout << "\nInvalid tableau move.\n";
      }

    } else if (command == 'A' || command == 'a') {
      int destination;

      std::cout << "Destination column (1-7): ";
      std::cin >> destination;

      GameBoard previousState = board;

      if (board.moveWasteToTableau(destination - 1)) {
        history.record(previousState);
        std::cout << "\nWaste card moved to tableau!\n";
      } else {
        std::cout << "\nInvalid waste-to-tableau move.\n";
      }

    } else if (command == 'F' || command == 'f') {
      GameBoard previousState = board;

      if (board.moveWasteToFoundation()) {
        history.record(previousState);
        std::cout << "\nWaste card moved to foundation!\n";
      } else {
        std::cout << "\nInvalid waste-to-foundation move.\n";
      }

    } else if (command == 'G' || command == 'g') {
      int source;

      std::cout << "Source column (1-7): ";
      std::cin >> source;

      GameBoard previousState = board;

      if (board.moveTableauToFoundation(source - 1)) {
        history.record(previousState);
        std::cout << "\nTableau card moved to foundation!\n";
      } else {
        std::cout << "\nInvalid tableau-to-foundation move.\n";
      }

    } else if (command == 'U' || command == 'u') {
      if (history.undo(board)) {
        std::cout << "\nLast move undone.\n";
      } else {
        std::cout << "\nNothing to undo.\n";
      }

    } else if (command == 'Y' || command == 'y') {
      if (history.redo(board)) {
        std::cout << "\nMove restored.\n";
      } else {
        std::cout << "\nNothing to redo.\n";
      }

    } else if (command == 'Q' || command == 'q') {
      std::cout << "\nThanks for playing!\n";
      break;

    } else {
      std::cout << "\nInvalid command.\n";
    }

    if (board.hasWon()) {
      board.display();
      std::cout << "\n🎉 CONGRATULATIONS! YOU WON! 🎉\n";
      break;
    }
  }

  return 0;
}
