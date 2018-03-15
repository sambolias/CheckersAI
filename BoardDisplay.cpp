#include "BoardDisplay.h"
#include "DistributionTestDisplay.h"
#include "NormalDistribution.h"
#include "UniformDistribution.h"
#include "LoadedGameDisplay.hpp"
#include <QDebug>
#include <QSharedPointer>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
using std::vector;

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
	connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));

	//exit application todo check to save or export your files
	QAction* quitAction = menuGame->addAction("Quit");
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	// Tests
	QMenu* menuTests = new QMenu("Tests");
	// Normal Distribution
	QAction* normalDistributionTestAction = menuTests->addAction("Normal Distribution Test");
	connect(normalDistributionTestAction, SIGNAL(triggered()), this, SLOT(normalDistributionTest()));
	// Uniform Distribution
	QAction* uniformDistributionTestAction = menuTests->addAction("Uniform Distribution Test");
	connect(uniformDistributionTestAction, SIGNAL(triggered()), this, SLOT(uniformDistribtutionTest()));
	

	QMenuBar* mainMenu = this->menuBar();
	mainMenu->addMenu(menuGame);
	mainMenu->addMenu(menuTests);

	// text display
	textDisplay = new QTextEdit(this);
	textDisplay->setFontPointSize(10);
	textDisplay->setReadOnly(true);
	textDisplay->setGeometry((H_BUFFER * 2) + (TILE_SIZE * 8), V_BUFFER + 20, 200, TILE_SIZE * 8);
	// reset boards
	resetBoards();
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
	QString filename = QFileDialog::getOpenFileName(this, tr("Game files"), ".txt");
	if (filename.isEmpty()) return;

	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		return;
	}
	QStringList fileContents;
	QTextStream in(&file);
	while (!in.atEnd())
	{
		fileContents.push_back(in.readLine());
	}

	QString winner = fileContents.front();
	fileContents.pop_front();
	QString redPlayer = fileContents.front();
	fileContents.pop_front();
	QString blackPlayer = fileContents.front();
	fileContents.pop_front();
	displayText(winner.toStdString());
	displayText(redPlayer.toStdString());
	displayText(blackPlayer.toStdString());
	_boards.clear();
	for (const auto & line : fileContents)
	{
		vector<char> board;
		for (const auto & boardPiece : line)
		{
			if (boardPiece != ' ')
			{
				board.push_back(boardPiece.toLatin1());
			}
		}
		Board b(board);
		_boards.push_back(b.GetBoardAsMatrix());
	}

	_currentBoard = _boards.begin();
	displayPieces(*_currentBoard);
}

void BoardDisplay::normalDistributionTest()
{
	double mean = 0;
	double deviation = 1;
	double increment = 0.25;
	int amount = 1700;
	QSharedPointer<StatisticalDistribution> distribution = QSharedPointer<NormalDistribution>::create(mean, deviation);
	DistributionTestDisplay * distributionTestDisplay = new DistributionTestDisplay(this);
	distributionTestDisplay->start(distribution, amount, increment);
}

void BoardDisplay::uniformDistribtutionTest()
{
	double lowerBound = -1.0;
	double upperBound = 1.0;
	double increment = 0.1;
	int amount = 1000;
	QSharedPointer<StatisticalDistribution> distribution = QSharedPointer<UniformDistribution>::create(lowerBound, upperBound);
	DistributionTestDisplay * distributionTestDisplay = new DistributionTestDisplay(this);
	distributionTestDisplay->start(distribution, amount, increment);
}

void BoardDisplay::keyPressEvent(QKeyEvent *event)
{
	if (_boards.empty())
		return;

	if (event->key() == Qt::Key_D)
	{
		if (_currentBoard < _boards.end() - 1)
			_currentBoard++;
	}
	else if (event->key() == Qt::Key_A)
	{
		if (_currentBoard > _boards.begin())
			_currentBoard--;
	}
	displayPieces(*_currentBoard);
}

void BoardDisplay::addBoard(const std::vector<std::vector<char>> & board)
{
	_boards.push_back(board);
	_currentBoard = _boards.end() - 1;
}

void BoardDisplay::resetBoards()
{
	_boards.clear();
	_currentBoard = _boards.begin();
}
