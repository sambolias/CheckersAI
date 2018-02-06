#include "ComputerPlayer.h"
#include <QDebug>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;


Board & ComputerPlayer::TakeTurn(Board & board, vector<shared_ptr<Movement>> & moves)
{
	if (moves.size())
	{
		int moveIndex = rand() % moves.size();
		auto move = moves[moveIndex];
		move->ToString();
		board = move->ExecuteMovement(board);
		return board.UpdateKings();
	}
	else
	{
		qDebug() << "There are no moves for " + _color << '\n';
		return board;
	}
}

