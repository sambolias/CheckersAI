#include "HumanPlayer.h"
#include <QDebug>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;

Board& HumanPlayer::TakeTurn(Board& board, vector<shared_ptr<Movement>>& moves)
{
	if (moves.size())
	{
		for (auto move : moves)
		{
			if (move->GetStartPosition() == move_start && move->GetEndPosition() == move_end)
			{
				move->ToString();
				board = move->ExecuteMovement(board);
				return board.UpdateKings();
			}
		}
	}
	else
	{
		qDebug() << "There are no moves for " + _color << '\n';
		return board;
	}
}

bool HumanPlayer::ValidMove(std::vector<std::shared_ptr<Movement>>& moves)
{
	for (auto move : moves)
	{
		if (move->GetStartPosition() == move_start && move->GetEndPosition() == move_end)
		{
			return true;
		}
	}
	return false;
}
