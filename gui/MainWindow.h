#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GameBoard.h"
#include "GameHistory.h"

class QLabel;
class QPushButton;
class QWidget;
class QGridLayout;

class MainWindow : public QMainWindow {
public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  GameBoard board;
  GameHistory history;

  QPushButton *stockButton;
  QPushButton *wasteButton;
  QPushButton *foundationButtons[4];

  QPushButton *drawOneButton;
  QPushButton *drawThreeButton;

  QWidget *tableauWidget;
  QGridLayout *tableauLayout;
  QLabel *statusLabel;

  int drawCount = 1;
  int selectedColumn = -1;
  int selectedPosition = -1;
  bool wasteSelected = false;

  void buildInterface();
  void refresh();
  void refreshTableau();

  void stockClicked();
  void wasteClicked();
  void tableauClicked(int column, int position);
  void foundationClicked(int suit);

  void newGame();
  void undoMove();
  void redoMove();

  QString cardText(const Card &card) const;
  QString cardStyle(const Card &card, bool selected = false) const;

  void clearSelection();
};

#endif
