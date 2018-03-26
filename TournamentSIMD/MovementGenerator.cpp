#include "MovementGenerator.hpp"
#include <iostream>
using std::vector;
using std::make_shared;
using std::shared_ptr;
using std::unordered_map;

MovementGenerator::MovementGenerator()
{
	_redMovementList = make_shared<RedMovementList>();
	_blackMovementList = make_shared<BlackMovementList>();
	_kingMovementList = make_shared<KingMovementList>();
}

vector<shared_ptr<Movement>> MovementGenerator::GenerateMoves(Board & board, char playerColor)
{
	bool playerIsRed = (playerColor == Board::RED);
	_playerColor = playerColor;
	_opponentColor = (playerIsRed) ? Board::BLACK : Board::RED;
	_playerMovementList = (playerIsRed) ? _redMovementList : _blackMovementList;

	vector<shared_ptr<Movement>> movements;
	// Get all jump moves
	movements = getAllJumpMovements(board);
	// If no jumps are available, get all regular moves
	if (movements.empty())
	{
		movements = getAllRegularMovements(board);
	}
	return movements;
}

vector<shared_ptr<Movement>> MovementGenerator::getAllJumpMovements(Board & board)
{
	vector<shared_ptr<Movement>> movements;
	for (int boardIndex = 0; boardIndex < board.GetBoard().size(); ++boardIndex)
	{
		if (tolower(board[boardIndex]) == _playerColor)
		{
			if (islower(board[boardIndex]))
			{
				movements = generateJumpMovements(movements, board, boardIndex, _playerMovementList);
			}
			else
			{
				movements = generateJumpMovements(movements, board, boardIndex, _kingMovementList);
			}
		}
	}
	return movements;
}

vector<shared_ptr<Movement>> MovementGenerator::getAllRegularMovements(Board & board)
{
	vector<shared_ptr<Movement>> movements;
	for (int boardIndex = 0; boardIndex < board.GetBoard().size(); ++boardIndex)
	{
		if (tolower(board[boardIndex]) == _playerColor)
		{
			if (islower(board[boardIndex]))
			{
				movements = generateRegularMovements(movements, board, boardIndex, _playerMovementList);
			}
			else
			{
				movements = generateRegularMovements(movements, board, boardIndex, _kingMovementList);
			}
		}
	}
	return movements;
}

vector<shared_ptr<Movement>> MovementGenerator::generateJumpMovements(vector<shared_ptr<Movement>> & movements, Board & board, int startPoint, shared_ptr<MovementList> & movementList)
{
	if (movementList->HasLegalRegularMoves(startPoint))
	{
		auto movementEndPoints = movementList->GetLegalRegularMoves(startPoint); //_legalRegularMoves[startPoint];
		for (auto endPoint : movementEndPoints)
		{
			// Check for jumps
			if (tolower(board[endPoint]) == _opponentColor)
			{
				int opponentPosition = endPoint;
				int jumpEndPoint = getJumpEndPoint(startPoint, opponentPosition, movementList);
				if (jumpEndPoint >= 0 && board[jumpEndPoint] == Board::EMPTY)
				{
					auto jumpMovement = make_shared<JumpMovement>(startPoint, jumpEndPoint, opponentPosition);
					movements = generateJumps(movements, board, jumpMovement, movementList);
				}
			}
		}
	}
	return movements;
}

vector<shared_ptr<Movement>> MovementGenerator::generateRegularMovements(vector<shared_ptr<Movement>> & movements, Board & board, int startPoint, shared_ptr<MovementList> & movementList)
{
	if (movementList->HasLegalRegularMoves(startPoint))
	{
		auto movementEndPoints = movementList->GetLegalRegularMoves(startPoint);
		for (auto endPoint : movementEndPoints)
		{
			if (board[endPoint] == Board::EMPTY)
			{
				auto move = make_shared<RegularMovement>(startPoint, endPoint);
				movements.push_back(move);
			}
		}
	}
	return movements;
}

vector<shared_ptr<Movement>> & MovementGenerator::generateJumps(vector<shared_ptr<Movement>> & jumpMoves,
                                                                Board & board, 
                                                                shared_ptr<JumpMovement> jumpMovement,
																shared_ptr<MovementList> & movementList)
{
	// starting from the end of the last jump
    int startPosition = jumpMovement->GetEndPosition(); 
	bool anotherJumpExists = false;
    if (movementList->HasLegalRegularMoves(startPosition))
    {
		auto postionsOneAway = movementList->GetLegalRegularMoves(startPosition);
        for (auto positionOneAway : postionsOneAway)
        {
            // check if position one away from start is opponent and if it hasn't been jumped already
            if (tolower(board[positionOneAway]) == _opponentColor && jumpMovement->ContainsJumpedPiece(positionOneAway) == false)
            {
                int opponentPosition = positionOneAway;
                int jumpEndPoint = getJumpEndPoint(startPosition, opponentPosition, movementList);
                if (jumpEndPoint >= 0 && board[jumpEndPoint] == Board::EMPTY)
                {
					anotherJumpExists = true;
                    int start = jumpMovement->GetStartPosition();
                    auto newJump = make_shared<JumpMovement>(start, jumpEndPoint, opponentPosition);
                    newJump->AddJumpedPieces(std::move(jumpMovement->GetJumpedPieces()));
                    jumpMoves = generateJumps(jumpMoves, board, newJump, movementList);
                }
            }
        }
    }
	if (anotherJumpExists == false)
	{
		jumpMoves.push_back(jumpMovement);
	}
    return jumpMoves; 
}

// finds the common space between a player's jump point and the opponent's starting position
// ex. for RED to jump from 1-10, there needs to be an opponent at space 5
int MovementGenerator::getJumpEndPoint(int playerStartPoint, int opponentStartPoint, shared_ptr<MovementList> & movementList)
{
	if (movementList->HasLegalJumpMoves(playerStartPoint) && movementList->HasLegalRegularMoves(opponentStartPoint))
	{
		auto jumpEndPoints = movementList->GetLegalJumpMoves(playerStartPoint); 
		auto opponentEndPoints = movementList->GetLegalRegularMoves(opponentStartPoint);
		for (auto jumpEndPoint : jumpEndPoints)
		{
			for (auto opponentEndPoint : opponentEndPoints)
			{
				if (jumpEndPoint == opponentEndPoint)
				{
					return jumpEndPoint;
				}
			}
		}
	}
	return -1;
}