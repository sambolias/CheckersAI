#include "KingMovementList.hpp"

using std::unordered_map;
using std::vector;

KingMovementList::KingMovementList()
{

}

bool KingMovementList::HasLegalJumpMoves(int key)
{
	return (_blackMovementList.HasLegalJumpMoves(key) || _redMovementList.HasLegalJumpMoves(key));
}

bool KingMovementList::HasLegalRegularMoves(int key)
{
	return (_blackMovementList.HasLegalRegularMoves(key) || _redMovementList.HasLegalRegularMoves(key));
}

vector<int> KingMovementList::GetLegalJumpMoves(int key)
{
	vector<int> blackJumpMoves, redJumpMoves;
	blackJumpMoves = _blackMovementList.GetLegalJumpMoves(key);
	redJumpMoves = _redMovementList.GetLegalJumpMoves(key);
	// Add red moves to black moves, then return black moves
	blackJumpMoves.insert(blackJumpMoves.end(), redJumpMoves.begin(), redJumpMoves.end());
	return blackJumpMoves;
}

vector<int> KingMovementList::GetLegalRegularMoves(int key)
{
	vector<int> blackRegularMoves, redRegularMoves;
	blackRegularMoves = _blackMovementList.GetLegalRegularMoves(key);
	redRegularMoves = _redMovementList.GetLegalRegularMoves(key);
	// Add red moves to black moves, then return black moves
	blackRegularMoves.insert(blackRegularMoves.end(), redRegularMoves.begin(), redRegularMoves.end());
	return blackRegularMoves;
}
