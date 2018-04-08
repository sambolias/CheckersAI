#include "ComputerPlayer.h"
#include <QDebug>
#include <algorithm>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
#include <limits>
using std::numeric_limits;

ComputerPlayer::ComputerPlayer(char color) : Player(color)
{
	qDebug() << "using computer player\n";
	_infinity = std::numeric_limits<double>::infinity();
}

Board& ComputerPlayer::TakeTurn(Board& board, vector<shared_ptr<Movement>>& moves)
{
	if (moves.size())
	{
		auto move = minimax(board, moves, 10);
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
	double bestValue = -_infinity;//-INFINITY;
	for (auto move : moves)
	{
		Board newBoard = board; // create new board because move->ExecuteMovement(board) modifies board
		double val = minimax(0, move->ExecuteMovement(newBoard).UpdateKings(), -_infinity, _infinity, false, depth);
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

double ComputerPlayer::minimax(double currentValue, Board & board, double alpha, double beta, bool maximize, int depth)
{
	// Add current board score to total
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
	double best = (maximize) ? -_infinity : _infinity; // set best to worst possible value
	for (shared_ptr<Movement> move : moves)
	{
		Board newBoard = board; // create new board because move->ExecuteMovement(board) modifies board
		double val = minimax(currentValue, move->ExecuteMovement(newBoard).UpdateKings(), alpha, beta, !maximize, depth - 1);
		best = (maximize) ? std::max(val, best) : std::min(val, best);
		if (maximize)
		{
			alpha = std::max(alpha, val);
		}
		else
		{
			beta = std::min(beta, val);
		}
		if (beta <= alpha)
		{
			break;
		}
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