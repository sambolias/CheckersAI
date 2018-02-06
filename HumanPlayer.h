#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.hpp"

class HumanPlayer : public Player
{
public:
	int move_start;
	int move_end;

	HumanPlayer(char color) : Player(color) {}
	Board & TakeTurn(Board & board, std::vector<std::shared_ptr<Movement>> & moves);
	bool ValidMove(std::vector<std::shared_ptr<Movement>> & moves);

};

#endif