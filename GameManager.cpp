#include "GameManager.h"
#include "BoardDisplay.h"
#include "ComputerPlayer.h"

// temporary to show use of gui
std::vector<std::vector<char>> generateBoard()
{
	std::vector<std::vector<char>> board(8, std::vector<char>(8));
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			board[x][y] = Board::EMPTY;
		}
	}
	return board;
}


void GameManager::startNewGame()
{
	std::shared_ptr<Player> playerOne = std::make_shared<HumanPlayer>(Board::RED);
	std::shared_ptr<Player> playerTwo = std::make_shared<ComputerPlayer>(Board::BLACK);
	Game game = Game(playerOne, playerTwo);

	display->displayPieces(game.GetBoard());
	playing = true;

	while (playing)
	{
		QApplication::processEvents();

		// if a human is playing, waits for their move and stores it in the HumanPlayer object
		Player& player = game.GetCurrentPlayer();
		if (typeid(player) == typeid(HumanPlayer))
		{
			HumanPlayer& human = dynamic_cast<HumanPlayer&>(player);
			human.move_start = move_start;
			human.move_end = move_end;
			// wait until a move is selected
			if (!moved)
			{
				continue;
			}
			else
			{
				moved = false;
			}
		}
		game.TakeNextTurn();
		if (game.IsOver())
			break;

		display->displayPieces(game.GetBoard());
	}

	std::string winner = ((game.GetTurn() == game.RED_TURN) ? "BLACK" : "RED");
	BoardDisplay::displayText(winner + " is the winner!");
}


void GameManager::onTileClicked(int x, int y)
{
	// convert the x,y tile grid index to the boards index to check for valid movement
	int board_index = (x + y * 8) / 2;
	if (!pieceSelected)
	{
		move_start = board_index;
		pieceSelected = true;
		std::cout << "Piece selected at index " << board_index << std::endl;
	}
	else
	{
		move_end = board_index;
		pieceSelected = false;
		moved = true;
		std::cout << "End location set to " << board_index << std::endl;
	}
	std::cout << x << ", " << y << std::endl;
}
