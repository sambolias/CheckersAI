#include "Player.hpp"
#include <QDebug>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;

vector<shared_ptr<Movement>> Player::GenerateMoves(Board & board)
{
	auto moves = _movementGenerator.GenerateMoves(board, _color);
	return moves;
}

vector<shared_ptr<Movement>> Player::GenerateMoves(Board & board, char color)
{
	auto moves = _movementGenerator.GenerateMoves(board, color);
	return moves;
}