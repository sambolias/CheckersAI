#ifndef JUMP_MOVEMENT_H_INCLUDED
#define JUMP_MOVEMENT_H_INCLUDED

#include "Movement.hpp"
#include <map>
#include <string>

class JumpMovement : public Movement
{
private:
    std::map<int,bool> _jumpedPieces;
    bool isValidJumpedPiece(int jumpedPiece);
public:
    JumpMovement(int start, int end) : Movement(start, end) {}
    JumpMovement(int start, int end, int jumpedPiece);
    std::map<int,bool> GetJumpedPieces();
    void AddJumpedPiece(int jumpedPiece);
    void AddJumpedPieces(std::map<int,bool> && jumpedPieces);
    bool ContainsJumpedPiece(int jumpedPiece);
    Board & ExecuteMovement(Board & board) override;
    std::string ToString() override;
};

#endif