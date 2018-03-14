#pragma once
#include <vector>
#include <memory>
#include <string>

class TournamentFileHandler
{
private:
	std::string _baseNetworkFileName;
    std::string _baseGenerationFolderPath;
    std::string _baseTournamentFolderPath;
    std::string _baseGameFileName;
	bool makeDirectory(const std::string & directoryPath);
public:
	TournamentFileHandler();
	void WriteGameToFile(int generation, int gameNumber, const std::string & winnerName, const std::string & redPlayerName, const std::string & blackPlayerName, const std::vector<std::vector<char>> & boards);
};

