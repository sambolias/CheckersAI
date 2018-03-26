#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Game.hpp"
#include <ostream>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <QApplication>
#include "HumanPlayer.h"

class BoardDisplay;

class GameManager
{
private:
	bool pieceSelected = false;
	int move_start;
	int move_end;
public:
	bool moved = false;
	bool playing;
	BoardDisplay* display;

	GameManager(BoardDisplay* display_) : display(display_), playing(false) {}
	void startNewGame(std::shared_ptr<Player> redPlayer, std::shared_ptr<Player> blackPlayer);
	void onTileClicked(int x, int y);
};

#endif
