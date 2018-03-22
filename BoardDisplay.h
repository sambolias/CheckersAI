#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

#include "TileDisplay.h"
#include "GameManager.h"
#include "Player.hpp"
#include "Board.hpp"
#include <QWidget>
#include <QAction>
#include <QTextEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QKeyEvent>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class BoardDisplay : public QMainWindow
{
	Q_OBJECT
private:
	virtual QSize sizeHint() const { return QSize(1000, 700); }
	const int V_BUFFER = 100;
	const int H_BUFFER = 75;
	const int TILE_SIZE = 64;
	std::vector<std::vector<std::vector<char>>> _boards;
	std::vector<std::vector<std::vector<char>>>::iterator _currentBoard;
	std::shared_ptr<Player> _redPlayer;
	std::shared_ptr<Player> _blackPlayer;
	enum _playerType;
	QMenu * createPlayerMenu(std::shared_ptr<Player> player, std::string menuName);

protected:
	void keyPressEvent(QKeyEvent * event);

public slots:
	void start();
	void save();
	void load();
	void quit();
	void normalDistributionTest();
	void uniformDistribtutionTest();
	void setRedHumanPlayer();
	void setRedHeuristicPlayer();
	void setRedNeuralNetworkPlayer();
	void setBlackHumanPlayer();
	void setBlackHeuristicPlayer();
	void setBlackNeuralNetworkPlayer();

public:
	TileDisplay * tiles[8][8];
	QWidget* widget;
	GameManager * manager;
	static QTextEdit* textDisplay;

	BoardDisplay();

	void display();
	void displayPieces(std::vector<std::vector<char>> board);
	static void displayText(std::string text);
	void addBoard(const std::vector<std::vector<char>> & board);
	void resetBoards();
};
#endif