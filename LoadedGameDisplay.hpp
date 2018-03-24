#ifndef LOADED_GAME_DISPLAY
#define LOADED_GAME_DISPLAY

#include <vector>
#include <QString>
#include "BoardDisplay.h"

class LoadedGameDisplay
{
private:
	BoardDisplay * _display;
	std::vector<std::vector<std::vector<char>>> _boards;
	std::vector<std::vector<std::vector<char>>>::iterator _currentBoard;
protected:
public:
	LoadedGameDisplay(BoardDisplay * display, QStringList gameFileContents);
};

#endif