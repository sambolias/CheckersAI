#include "TournamentFileHandler.h"
#include <string>
using std::string;
using std::to_string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <sstream>
using std::stringstream;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <exception>
using std::exception;
#include <windows.h>

TournamentFileHandler::TournamentFileHandler()
{
	char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string workingDirectory(buffer);
    workingDirectory = workingDirectory.substr(0, workingDirectory.length() - 5); // take of a.exe from the path

	_baseNetworkFileName = "Network_";
    _baseGenerationFolderPath = workingDirectory + "B24\\Generation_";
    _baseTournamentFolderPath = workingDirectory + "B24\\Tournament_";
    _baseGameFileName = "Game_";
}

bool TournamentFileHandler::makeDirectory(const string & directoryPath)
{
    // https://stackoverflow.com/questions/9235679/create-a-directory-if-it-doesnt-exist
    LPCWSTR path = std::wstring(directoryPath.begin(), directoryPath.end()).c_str();
    if (CreateDirectoryW(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return true;
    }
    else
    {
        //cout << "Failed to create directory " << directoryPath << endl;
        return false;
    }
}

void TournamentFileHandler::WriteGameToFile(int generation, int gameNumber, const string & winnerName, const string & redPlayerName, const string & blackPlayerName, const vector<vector<char>> & boards)
{
	string tournamentPath = _baseTournamentFolderPath + to_string(generation);
	makeDirectory(tournamentPath);

	string gamePath = tournamentPath + '\\' + _baseGameFileName + to_string(gameNumber) + ".txt";

	ofstream file(gamePath);
	if (file.is_open())
	{
		file << "Winner: " << winnerName << '\n';
		file << "Red: " << redPlayerName << '\n';
		file << "Black: " << blackPlayerName << '\n';
		for (const auto & board : boards)
		{
			for (char c : board)
			{
				file << c << ' ';
			}
			file << '\n';
		}
		file.close();
	}
}