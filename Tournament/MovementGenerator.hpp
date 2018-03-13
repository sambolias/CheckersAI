#ifndef MOVEMENT_GENERATOR_H_INCLUDED
#define MOVEMENT_GENERATOR_H_INCLUDED

#include "Board.hpp"
#include "MovementList.hpp"
#include "RedMovementList.hpp"
#include "BlackMovementList.hpp"
#include "KingMovementList.hpp"
#include "Movement.hpp"
#include "RegularMovement.hpp"
#include "JumpMovement.hpp"
#include <vector>
#include <unordered_map>
#include <memory> // shared_ptr, make_shared
#include <utility>
#include <locale> // tolower

class MovementGenerator
{
private:
	char _playerColor;
	char _opponentColor;
	int getJumpEndPoint(int playerStartPoint, int opponentStartPoint, std::shared_ptr<MovementList> & movementList);
	std::shared_ptr<MovementList> _redMovementList;
	std::shared_ptr<MovementList> _blackMovementList;
	std::shared_ptr<MovementList> _kingMovementList;
	std::shared_ptr<MovementList> _playerMovementList;
	std::vector<std::shared_ptr<Movement>> getAllJumpMovements(Board & board);
	std::vector<std::shared_ptr<Movement>> getAllRegularMovements(Board & board);
	std::vector<std::shared_ptr<Movement>> generateJumpMovements(std::vector<std::shared_ptr<Movement>> & movements, Board & board, int startPoint, std::shared_ptr<MovementList> & movementList);
	std::vector<std::shared_ptr<Movement>> generateRegularMovements(std::vector<std::shared_ptr<Movement>> & movements, Board & board, int startPoint, std::shared_ptr<MovementList> & movementList);
	std::vector<std::shared_ptr<Movement>> & generateJumps(std::vector<std::shared_ptr<Movement>> & jumpMoves, Board & board,
														   std::shared_ptr<JumpMovement> jumpMovement, std::shared_ptr<MovementList> & movementList);
public:
	MovementGenerator();
    // Generates a list of moves (needs to still generate jump moves)
	std::vector<std::shared_ptr<Movement>> GenerateMoves(Board & board, char playerColor);	
};

#endif