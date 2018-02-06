#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Board.hpp"
#include "MovementGenerator.hpp"
#include <vector>
#include <memory>
#include <iostream>

class Player
{
protected:
    char _color;
    bool _moved;
    MovementGenerator _movementGenerator;
public:
    Player(char color) : _color(color) {}
    std::vector<std::shared_ptr<Movement>> GenerateMoves(Board & board);
    virtual Board & TakeTurn(Board & board, std::vector<std::shared_ptr<Movement>> & moves) = 0;
    virtual bool ValidMove(std::vector<std::shared_ptr<Movement>> & moves) = 0;
};

#endif