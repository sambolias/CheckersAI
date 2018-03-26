#ifndef MOVEMENT_LIST_H_INCLUDED
#define MOVEMENT_LIST_H_INCLUDED

#include <unordered_map>
#include <vector>

class MovementList
{
protected:
    std::unordered_map<int,std::vector<int>> _legalRegularMoves;
    std::unordered_map<int,std::vector<int>> _legalJumpMoves;
public:
	virtual ~MovementList() = default;
	virtual bool HasLegalJumpMoves(int key);
	virtual bool HasLegalRegularMoves(int key);
	virtual std::vector<int> GetLegalJumpMoves(int key);
	virtual std::vector<int> GetLegalRegularMoves(int key);
};

#endif