#ifndef GAMEHISTORY_H
#define GAMEHISTORY_H

#include "GameBoard.h"
#include "Stack.h"

class GameHistory {
private:
  static constexpr int HISTORY_CAPACITY = 100;

  Stack<GameBoard, HISTORY_CAPACITY> undoHistory;
  Stack<GameBoard, HISTORY_CAPACITY> redoHistory;

public:
  // Save a snapshot taken before a successful move.
  void record(const GameBoard &previousState) {
    undoHistory.push(previousState);
    redoHistory.clear();
  }

  // Restore the previous state.
  bool undo(GameBoard &board) {
    if (undoHistory.isEmpty()) {
      return false;
    }

    redoHistory.push(board);
    board = undoHistory.pop();

    return true;
  }

  // Restore a state that was previously undone.
  bool redo(GameBoard &board) {
    if (redoHistory.isEmpty()) {
      return false;
    }

    undoHistory.push(board);
    board = redoHistory.pop();

    return true;
  }

  void clear() {
    undoHistory.clear();
    redoHistory.clear();
  }

  bool canUndo() const { return !undoHistory.isEmpty(); }

  bool canRedo() const { return !redoHistory.isEmpty(); }
};

#endif
