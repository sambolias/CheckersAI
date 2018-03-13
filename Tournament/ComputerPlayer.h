#ifndef COMPUTER_PLAYER_H_INCLUDED
#define COMPUTER_PLAYER_H_INCLUDED

#include "Player.hpp"

class ComputerPlayer : public Player
{
public:
	ComputerPlayer(char color) : Player(color) {}
	Board & TakeTurn(Board & board, std::vector<std::shared_ptr<Movement>> & moves);
	bool ValidMove(std::vector<std::shared_ptr<Movement>> & moves) { return true; }
	std::shared_ptr<Movement> minimax(Board board, std::vector<std::shared_ptr<Movement>>& moves, int depth);
	double minimax(double currentValue, Board & board, bool maximize, int depth);
	virtual double getHeuristic(Board & board);
};



#endif