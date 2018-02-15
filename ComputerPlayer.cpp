#include "ComputerPlayer.h"
#include <QDebug>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;


Board& ComputerPlayer::TakeTurn(Board& board, vector<shared_ptr<Movement>>& moves)
{
	if (moves.size())
	{
		auto move = minimax(board, moves, 6);
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

shared_ptr<Movement> ComputerPlayer::minimax(Board board, vector<shared_ptr<Movement>>& moves, int depth)
{
	vector<std::tuple<shared_ptr<Movement>, double>> weighted;
	for (shared_ptr<Movement> move : moves)
	{
		double val = minimax(move->ExecuteMovement(board).UpdateKings(), true, depth);
		weighted.emplace_back(std::make_tuple(move, val));
	}
	auto best = weighted[0];
	for (auto move : weighted)
	{
		if (std::get<1>(move) > std::get<1>(best))
		{
			best = move;
		}
	}
	return std::get<0>(best);
}

double ComputerPlayer::minimax(Board board, bool maximize, int depth)
{
	vector<shared_ptr<Movement>> moves = GenerateMoves(board);
	if (depth == 0 || GenerateMoves(board).size() == 0)
	{
		return getHeuristic(board);
	}

	if (maximize)
	{
		double best = INFINITY;
		for (shared_ptr<Movement> move : moves)
		{
			double val = minimax(move->ExecuteMovement(board).UpdateKings(), !maximize, depth - 1);
			return std::max(val, best);
		}
	}
	else
	{
		double best = -INFINITY;
		for (shared_ptr<Movement> move : moves)
		{
			double val = minimax(move->ExecuteMovement(board).UpdateKings(), !maximize, depth - 1);
			return std::min(val, best);
		}
	}
}

double ComputerPlayer::getHeuristic(Board& board)
{
	const double kingWeight = 2;
	double redValue = board.getPieceCount(Board::K_RED) * kingWeight + board.getPieceCount(Board::RED);
	double blackValue = board.getPieceCount(Board::K_BLACK) * kingWeight + board.getPieceCount(Board::BLACK);
	return _color == Board::RED ? redValue - blackValue : blackValue - redValue;
}
