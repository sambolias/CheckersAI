#include "MovementList.hpp"
using std::vector;

bool MovementList::HasLegalJumpMoves(int key)
{
	return (_legalJumpMoves.count(key));
}

bool MovementList::HasLegalRegularMoves(int key)
{
	return (_legalRegularMoves.count(key));
}

vector<int> MovementList::GetLegalJumpMoves(int key)
{
	if (_legalJumpMoves.count(key))
	{
		return _legalJumpMoves.at(key);
	}
	return {};
}

vector<int> MovementList::GetLegalRegularMoves(int key)
{
	if (_legalRegularMoves.count(key))
	{
		return _legalRegularMoves.at(key);
	}
	return {};
}