#include "RegularMovement.hpp"
using std::string;
using std::to_string;

Board & RegularMovement::ExecuteMovement(Board & board)
{
    board[_end] = board[_start];
    board[_start] = Board::EMPTY;
    return board;
}

string RegularMovement::ToString()
{
    return "Regular Movement (start - end): " + to_string(_start) + " - " + to_string(_end);
}
