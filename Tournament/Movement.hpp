#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED

#include "Board.hpp"
#include <string>

class Movement
{
protected:
	int _start;
	int _end;
public:

	Movement(int start, int end) : _start(start), _end(end) {}

	virtual Board& ExecuteMovement(Board& board) = 0;
	virtual std::string ToString() { return ""; }

	int GetStartPosition() { return _start; }
	int GetEndPosition() { return _end; }
	void SetStartPosition(int start) { _start = start; }
	void SetEndPosition(int end) { _end = end; }
};

#endif
