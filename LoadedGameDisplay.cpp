#include "LoadedGameDisplay.hpp"
#include "Board.hpp"
#include <QDebug>
#include <string>
using std::string;
using std::vector;

LoadedGameDisplay::LoadedGameDisplay(BoardDisplay * display, QStringList gameFileContents)
{
	qDebug() << "File Contents";
	for (const auto & line : gameFileContents)
	{
		qDebug() << line;
	}
	_display = display;
	QString winner = gameFileContents.front();
	gameFileContents.pop_front();
	QString redPlayer = gameFileContents.front();
	gameFileContents.pop_front();
	QString blackPlayer = gameFileContents.front();
	gameFileContents.pop_front();
	BoardDisplay::displayText(winner.toStdString());
	BoardDisplay::displayText(redPlayer.toStdString());
	BoardDisplay::displayText(blackPlayer.toStdString());
	for (const auto & line : gameFileContents)
	{
		vector<char> board;
		//QStringList boardPieces = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		for (const auto & boardPiece : line)
		{
			if (boardPiece != ' ')
			{
				board.push_back(boardPiece.toLatin1());
			}
		}
		Board b(board);
		_boards.push_back(b.GetBoardAsMatrix());
	}

	_currentBoard = _boards.begin();
	_display->displayPieces(*_currentBoard);
}