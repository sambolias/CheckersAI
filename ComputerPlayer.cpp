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
		auto move = minimax(board, moves, 3);
		move->ToString();
		board = move->ExecuteMovement(board);
		return board.UpdateKings();
	}
	else
	{
		qDebug() << "There are no moves for " + _color;
		return board;
	}
}

shared_ptr<Movement> ComputerPlayer::minimax(Board board, vector<shared_ptr<Movement>>& moves, int depth)
{
	shared_ptr<Movement> bestMove = 0;
	double bestValue = -INFINITY;
	for (auto move : moves)
	{
		Board newBoard = board; // create new board because move->ExecuteMovement(board) modifies board
		double val = minimax(0, move->ExecuteMovement(newBoard).UpdateKings(), false, depth);
		if (val > bestValue)
		{
			bestMove = move;
			bestValue = val;
		}
		//qDebug() << "Move " << move->ToString().c_str() << " with weight " << val;
	}
	//qDebug() << "Chose move " << bestMove->ToString().c_str() << " with weight " << bestValue << '\n';
	return bestMove;
}

double ComputerPlayer::minimax(double currentValue, Board board, bool maximize, int depth)
{
	currentValue += getHeuristic(board);
	// Generate moves for either computer or opponent (based on maximize)
	char color = _color;
	if (!maximize)
	{
		color = (_color == Board::RED) ? Board::BLACK : Board::RED;
	}
	vector<shared_ptr<Movement>> moves = GenerateMoves(board, color);
	if (depth == 0 || moves.size() == 0)
	{
		return currentValue;
	}
	// Evaluate moves for best move
	double best = (maximize) ? -INFINITY : INFINITY; // set best to worst possible value
	for (shared_ptr<Movement> move : moves)
	{
		Board newBoard = board; // create new board because move->ExecuteMovement(board) modifies board
		double val = minimax(currentValue, move->ExecuteMovement(newBoard).UpdateKings(), !maximize, depth - 1);
		best = (maximize) ? std::max(val, best) : std::min(val, best);
	}
	return best;
}

double ComputerPlayer::getHeuristic(Board& board)
{
	const double kingWeight = 1.3;
	double redValue = board.getPieceCount(Board::K_RED) * kingWeight + board.getPieceCount(Board::RED);
	double blackValue = board.getPieceCount(Board::K_BLACK) * kingWeight + board.getPieceCount(Board::BLACK);
	return _color == Board::RED ? redValue - blackValue : blackValue - redValue;
}
