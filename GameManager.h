#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Game.hpp"
#include <ostream>
#include <iostream>
#include <vector>
#include <utility>
#include <QApplication>
#include "HumanPlayer.h"

extern class BoardDisplay;

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
	void startNewGame();
	void onTileClicked(int x, int y);
};

#endif
