#include "JumpMovement.hpp"
#include <iostream>
using std::map;
using std::string;
using std::to_string;

JumpMovement::JumpMovement(int start, int end, int jumpedPiece) : JumpMovement(start, end)
{
    AddJumpedPiece(jumpedPiece);
}

bool JumpMovement::isValidJumpedPiece(int jumpedPiece)
{
    return (jumpedPiece < 32 && jumpedPiece >= 0);
}

Board & JumpMovement::ExecuteMovement(Board & board)
{
    for (const auto & jumpedPiece : _jumpedPieces)
    {
    	int key = jumpedPiece.first;
    	board[key] = Board::EMPTY;
    }
    board[_end] = board[_start];
	board[_start] = Board::EMPTY;
    return board;
}

string JumpMovement::ToString()
{
	string returnString;
	returnString = "Jump Movement (start - [jumped pieces] - end): " + to_string(_start) + " - [";
    bool firstPiece = true;
    for (const auto jumpedPiece : _jumpedPieces)
    {
        if (firstPiece == false)
        {
			returnString += ", " + to_string(jumpedPiece.first);
        }
        else
        {
			returnString += to_string(jumpedPiece.first);
            firstPiece = false;
        }
    }
	returnString += "] - " + to_string(_end);
	return returnString;
}

map<int,bool> JumpMovement::GetJumpedPieces()
{
    return _jumpedPieces;
}

void JumpMovement::AddJumpedPiece(int jumpedPiece)
{
    if (isValidJumpedPiece(jumpedPiece))
    {
        _jumpedPieces[jumpedPiece] = true;
    }
}

void JumpMovement::AddJumpedPieces(map<int,bool> && jumpedPieces)
{
    for (const auto jumpedPiece : jumpedPieces)
    {
    	_jumpedPieces[jumpedPiece.first] = jumpedPiece.second;
    }
}


bool JumpMovement::ContainsJumpedPiece(int jumpedPiece)
{
    return (_jumpedPieces.count(jumpedPiece));
}