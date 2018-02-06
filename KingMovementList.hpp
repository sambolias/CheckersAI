#include "MovementList.hpp"
#include "BlackMovementList.hpp"
#include "RedMovementList.hpp"

#include <vector>

class KingMovementList : public MovementList
{
private:
	BlackMovementList _blackMovementList;
	RedMovementList	_redMovementList;
public:
    KingMovementList();
	virtual bool HasLegalJumpMoves(int key) override;
	virtual bool HasLegalRegularMoves(int key) override;
	virtual std::vector<int> GetLegalJumpMoves(int key) override;
	virtual std::vector<int> GetLegalRegularMoves(int key) override;
};