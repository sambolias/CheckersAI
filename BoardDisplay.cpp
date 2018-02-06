#include "BoardDisplay.h"

QTextEdit * BoardDisplay::textDisplay;

BoardDisplay::BoardDisplay()
{
	manager = new GameManager(this);
	widget = new QWidget(this);
	this->setCentralWidget(widget);

	// menu bar, mostly for later in the project when more options such as loading are required
	QMenu* menuGame = new QMenu("Game");
	QAction* startAction = menuGame->addAction("Start");
	connect(startAction, SIGNAL(triggered()), this, SLOT(start()));

	// save game
	QAction* saveAction = menuGame->addAction("Save");
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	// load game
	QAction* loadAction = menuGame->addAction("Load");
	connect(saveAction, SIGNAL(triggered()), this, SLOT(load()));

	//exit application todo check to save or export your files
	QAction* quitAction = menuGame->addAction("Quit");
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	QMenuBar* mainMenu = this->menuBar();
	mainMenu->addMenu(menuGame);

	// text display
	textDisplay = new QTextEdit(this);
	textDisplay->setFontPointSize(10);
	textDisplay->setReadOnly(true);
	textDisplay->setGeometry((H_BUFFER * 2) + (TILE_SIZE * 8), V_BUFFER + 20, 200, TILE_SIZE * 8);
}

void BoardDisplay::display()
{
	int horizontal = H_BUFFER;
	for (int x = 0; x < 8; x++)
	{
		int vertical = V_BUFFER;
		for (int y = 0; y < 8; y++)
		{
			tiles[x][y] = new TileDisplay(x, y, manager, widget);
			tiles[x][y]->color = (x + y) % 2;
			tiles[x][y]->display();
			tiles[x][y]->setGeometry(horizontal, vertical, TILE_SIZE, TILE_SIZE);
			vertical += TILE_SIZE;
		}
		horizontal += TILE_SIZE;
	}
	this->show();
}

void BoardDisplay::displayPieces(std::vector<std::vector<char>> board)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			// todo update this for whatever type of board is being passed in, but it should work as is
			char type = board[x][y];
			if (type == Board::EMPTY || type == Board::INVALID_SQUARE)
			{
				tiles[x][y]->removePiece();
			}
			else
			{
				int color = std::tolower(type) == Board::RED; // black = 0, red = 1
				bool king = std::isupper(type); // king or not
				tiles[x][y]->displayPiece(color, king);
			}
		}
	}
}

void BoardDisplay::displayText(std::string text)
{
	BoardDisplay::textDisplay->append(QString::fromStdString(text));
}

void BoardDisplay::start()
{
	displayText("New game started");
	manager->startNewGame();
}

void BoardDisplay::quit()
{
	std::exit(1);
}


void BoardDisplay::save()
{
}

void BoardDisplay::load()
{
}
