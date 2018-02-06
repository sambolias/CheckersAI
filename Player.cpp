#include "Player.hpp"
#include <QDebug>
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;

vector<shared_ptr<Movement>> Player::GenerateMoves(Board & board)
{
	auto moves = _movementGenerator.GenerateMoves(board, _color);
	qDebug() << ((_color == Board::BLACK) ? "BLACK's turn." : "RED's turn.");
	for (auto & move : moves)
	{
		qDebug() << QString((move->ToString()).c_str());
	}
	return moves;
}