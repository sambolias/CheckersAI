#ifndef COMPUTER_PLAYER_H_INCLUDED
#define COMPUTER_PLAYER_H_INCLUDED

#include "Player.hpp"

class ComputerPlayer : public Player
{
protected:
public:
	ComputerPlayer(char color) : Player(color) {}
	Board & TakeTurn(Board & board, std::vector<std::shared_ptr<Movement>> & moves);
	bool ValidMove(std::vector<std::shared_ptr<Movement>> & moves) { return true; }
};

#endif#pragma once
