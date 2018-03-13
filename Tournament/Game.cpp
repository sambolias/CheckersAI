#include "Game.hpp"
#include "Player.hpp"
using std::vector;
using std::string;
using std::to_string;
using std::cout;

Game::Game(std::shared_ptr<Player> playerOne, std::shared_ptr<Player> playerTwo) : RED_TURN(0), BLACK_TURN(1)
{
	_players.push_back(playerOne);
	_players.push_back(playerTwo);
	_turn = RED_TURN;
	_isOver = false;
}

bool Game::IsOver()
{
	return _isOver;

}

void Game::TakeNextTurn()
{
	auto moves = _players[_turn]->GenerateMoves(_board);
	if (moves.size())
	{
		_board = _players[_turn]->TakeTurn(_board, moves);
		_turn = (_turn == BLACK_TURN) ? RED_TURN : BLACK_TURN;
	}
	else
	{
		_isOver = true;
	}
}

vector<vector<char>> Game::GetBoard()
{
	return _board.GetBoardAsMatrix();
}

int Game::GetTurn()
{
	return _turn;
}

Player& Game::GetCurrentPlayer()
{
	return *_players[_turn];
}
