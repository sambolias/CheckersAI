#ifndef REGULAR_MOVEMENT_H_INCLUDED
#define REGULAR_MOVEMENT_H_INCLUDED

#include "Movement.hpp"
#include <string>

class RegularMovement : public Movement
{
public:
    RegularMovement(int start, int end) : Movement(start, end) {}
    Board & ExecuteMovement(Board & board) override;
    std::string ToString() override;
};

#endif