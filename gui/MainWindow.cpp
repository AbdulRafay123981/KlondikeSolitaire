#include "MainWindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  board.initialize();

  buildInterface();
  refresh();

  setWindowTitle("Klondike Solitaire");
  resize(900, 720);
}

void MainWindow::buildInterface() {
  QWidget *central = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(central);

  // Game controls
  QHBoxLayout *controls = new QHBoxLayout();

  QPushButton *newGameButton = new QPushButton("New Game");
  QPushButton *undoButton = new QPushButton("Undo");
  QPushButton *redoButton = new QPushButton("Redo");

  drawOneButton = new QPushButton("Draw 1");
  drawThreeButton = new QPushButton("Draw 3");

  drawOneButton->setCheckable(true);
  drawThreeButton->setCheckable(true);
  drawOneButton->setChecked(true);

  controls->addWidget(newGameButton);
  controls->addWidget(undoButton);
  controls->addWidget(redoButton);
  controls->addStretch();
  controls->addWidget(drawOneButton);
  controls->addWidget(drawThreeButton);

  mainLayout->addLayout(controls);

  connect(newGameButton, &QPushButton::clicked, this, [this]() { newGame(); });

  connect(undoButton, &QPushButton::clicked, this, [this]() { undoMove(); });

  connect(redoButton, &QPushButton::clicked, this, [this]() { redoMove(); });

  connect(drawOneButton, &QPushButton::clicked, this, [this]() {
    drawCount = 1;
    drawOneButton->setChecked(true);
    drawThreeButton->setChecked(false);
  });

  connect(drawThreeButton, &QPushButton::clicked, this, [this]() {
    drawCount = 3;
    drawThreeButton->setChecked(true);
    drawOneButton->setChecked(false);
  });

  // Stock, waste and foundations
  QHBoxLayout *piles = new QHBoxLayout();

  stockButton = new QPushButton();
  wasteButton = new QPushButton();

  stockButton->setFixedSize(76, 104);
  wasteButton->setFixedSize(76, 104);

  piles->addWidget(stockButton);
  piles->addWidget(wasteButton);
  piles->addStretch();

  connect(stockButton, &QPushButton::clicked, this,
          [this]() { stockClicked(); });

  connect(wasteButton, &QPushButton::clicked, this,
          [this]() { wasteClicked(); });

  for (int suit = 0; suit < 4; ++suit) {
    foundationButtons[suit] = new QPushButton();
    foundationButtons[suit]->setFixedSize(76, 104);

    piles->addWidget(foundationButtons[suit]);

    connect(foundationButtons[suit], &QPushButton::clicked, this,
            [this, suit]() { foundationClicked(suit); });
  }

  mainLayout->addLayout(piles);

  // Seven tableau columns
  tableauWidget = new QWidget();
  tableauLayout = new QGridLayout(tableauWidget);
  tableauLayout->setAlignment(Qt::AlignTop);
  tableauLayout->setHorizontalSpacing(12);
  tableauLayout->setVerticalSpacing(0);

  mainLayout->addWidget(tableauWidget);
  mainLayout->addStretch();

  statusLabel = new QLabel("Welcome to Klondike!");
  mainLayout->addWidget(statusLabel);

  setCentralWidget(central);
}

QString MainWindow::cardText(const Card &card) const {
  if (!card.isFaceUp()) {
    return "◆";
  }

  QString rank;

  switch (static_cast<int>(card.getRank())) {
  case 1:
    rank = "A";
    break;
  case 11:
    rank = "J";
    break;
  case 12:
    rank = "Q";
    break;
  case 13:
    rank = "K";
    break;
  default:
    rank = QString::number(static_cast<int>(card.getRank()));
    break;
  }

  QString suit;

  switch (card.getSuit()) {
  case Suit::Hearts:
    suit = "♥";
    break;
  case Suit::Diamonds:
    suit = "♦";
    break;
  case Suit::Spades:
    suit = "♠";
    break;
  case Suit::Clubs:
    suit = "♣";
    break;
  }

  return rank + "\n" + suit;
}

QString MainWindow::cardStyle(const Card &card, bool selected) const {
  QString color = "black";

  if (card.isFaceUp() &&
      (card.getSuit() == Suit::Hearts || card.getSuit() == Suit::Diamonds)) {
    color = "#c62828";
  }

  QString border = selected ? "3px solid #f5c542" : "1px solid #333333";

  if (!card.isFaceUp()) {
    return "QPushButton {"
           "background-color: #244f91;"
           "color: white;"
           "border: " +
           border +
           ";"
           "border-radius: 7px;"
           "font-size: 30px;"
           "}";
  }

  return "QPushButton {"
         "background-color: #fffdf5;"
         "color: " +
         color +
         ";"
         "border: " +
         border +
         ";"
         "border-radius: 7px;"
         "font-size: 22px;"
         "font-weight: bold;"
         "}";
}

void MainWindow::refresh() {
  // Stock
  if (board.getStockSize() > 0) {
    stockButton->setText("◆\n" + QString::number(board.getStockSize()));
    stockButton->setStyleSheet("background-color: #244f91;"
                               "color: white;"
                               "border: 2px solid white;"
                               "border-radius: 7px;"
                               "font-size: 22px;");
  } else {
    stockButton->setText("Recycle");
    stockButton->setStyleSheet("background-color: #315b3b;"
                               "color: white;"
                               "border: 1px dashed white;"
                               "border-radius: 7px;");
  }

  // Waste
  if (board.getWasteSize() > 0) {
    Card *card = board.getWasteTop();
    wasteButton->setText(cardText(*card));
    wasteButton->setStyleSheet(cardStyle(*card, wasteSelected));
  } else {
    wasteButton->setText("Waste");
    wasteButton->setStyleSheet("background-color: #315b3b;"
                               "color: white;"
                               "border: 1px dashed white;"
                               "border-radius: 7px;");
  }

  // Foundations
  const QString suitNames[4] = {"♥", "♦", "♠", "♣"};

  for (int suit = 0; suit < 4; ++suit) {
    int count = board.getFoundationSize(suit);

    if (count > 0) {
      const Card &card = board.getFoundationCard(suit, count - 1);
      foundationButtons[suit]->setText(cardText(card));
      foundationButtons[suit]->setStyleSheet(cardStyle(card));
    } else {
      foundationButtons[suit]->setText(suitNames[suit]);
      foundationButtons[suit]->setStyleSheet("background-color: #315b3b;"
                                             "color: white;"
                                             "border: 1px dashed white;"
                                             "border-radius: 7px;"
                                             "font-size: 26px;");
    }
  }

  refreshTableau();

  if (wasteSelected) {
    statusLabel->setText(
        "Waste selected. Choose a tableau column or foundation.");
  } else if (selectedColumn >= 0) {
    statusLabel->setText("Tableau card selected. Choose a destination.");
  } else {
    statusLabel->setText("Select a card, then select where to move it.");
  }
}

void MainWindow::refreshTableau() {
  // Remove the previous column widgets.
  while (QLayoutItem *item = tableauLayout->takeAt(0)) {
    if (QWidget *widget = item->widget()) {
      widget->deleteLater();
    }
    delete item;
  }

  for (int column = 0; column < 7; ++column) {
    int count = board.getTableauSize(column);

    QWidget *columnWidget = new QWidget(tableauWidget);
    columnWidget->setFixedWidth(78);

    int height = count == 0 ? 110 : 110 + (count - 1) * 27;
    columnWidget->setFixedHeight(height);

    if (count == 0) {
      QPushButton *emptyButton = new QPushButton(columnWidget);
      emptyButton->setGeometry(0, 0, 76, 104);
      emptyButton->setText("K");
      emptyButton->setStyleSheet("background-color: #315b3b;"
                                 "color: #d9e6d8;"
                                 "border: 1px dashed white;"
                                 "border-radius: 7px;");

      connect(emptyButton, &QPushButton::clicked, this,
              [this, column]() { tableauClicked(column, -1); });
    } else {
      for (int position = 0; position < count; ++position) {
        Card &card = board.getTableauCard(column, position);

        QPushButton *cardButton = new QPushButton(columnWidget);
        cardButton->setGeometry(0, position * 27, 76, 104);
        cardButton->setText(cardText(card));

        bool selected =
            selectedColumn == column && selectedPosition == position;

        cardButton->setStyleSheet(cardStyle(card, selected));

        connect(
            cardButton, &QPushButton::clicked, this,
            [this, column, position]() { tableauClicked(column, position); });
      }
    }

    tableauLayout->addWidget(columnWidget, 0, column);
  }
}

void MainWindow::clearSelection() {
  selectedColumn = -1;
  selectedPosition = -1;
  wasteSelected = false;
}

void MainWindow::stockClicked() {
  GameBoard previous = board;
  bool success = false;

  if (board.getStockSize() > 0) {
    success = board.drawFromStock(drawCount);
  } else {
    success = board.recycleWaste();
  }

  if (success) {
    history.record(previous);
    clearSelection();
    refresh();
  }
}

void MainWindow::wasteClicked() {
  if (board.getWasteSize() == 0) {
    return;
  }

  selectedColumn = -1;
  selectedPosition = -1;
  wasteSelected = !wasteSelected;

  refresh();
}

void MainWindow::tableauClicked(int column, int position) {
  // A selected waste card is moving to this column.
  if (wasteSelected) {
    GameBoard previous = board;

    if (board.moveWasteToTableau(column)) {
      history.record(previous);
      clearSelection();
      refresh();
    }

    return;
  }

  // A selected tableau sequence is moving to this column.
  if (selectedColumn >= 0) {
    if (selectedColumn == column) {
      clearSelection();
    } else {
      GameBoard previous = board;

      if (board.moveTableauSequence(selectedColumn, selectedPosition, column)) {
        history.record(previous);
        clearSelection();
      } else {
        // Failed move: select the newly clicked card instead.
        if (position >= 0 &&
            board.getTableauCard(column, position).isFaceUp()) {
          selectedColumn = column;
          selectedPosition = position;
        }
      }
    }

    refresh();
    return;
  }

  // Select a face-up tableau card.
  if (position >= 0 && board.getTableauCard(column, position).isFaceUp()) {
    selectedColumn = column;
    selectedPosition = position;
    refresh();
  }
}

void MainWindow::foundationClicked(int suit) {
  GameBoard previous = board;
  bool success = false;

  if (wasteSelected) {
    success = board.moveWasteToFoundation();
  } else if (selectedColumn >= 0 && selectedPosition == 0) {
    success = board.moveTableauToFoundation(selectedColumn);
  }

  if (success) {
    history.record(previous);
    clearSelection();
    refresh();
  }
}

void MainWindow::newGame() {
  board.initialize();
  history.clear();
  clearSelection();
  refresh();
}

void MainWindow::undoMove() {
  if (history.undo(board)) {
    clearSelection();
    refresh();
  }
}

void MainWindow::redoMove() {
  if (history.redo(board)) {
    clearSelection();
    refresh();
  }
}
