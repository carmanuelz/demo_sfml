#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

namespace sse
{
class Player : public Character
{
public:
    Player(float inx, float iny, std::string inCode, GameContext* incontext):Character(inx,iny,inCode,incontext)
    {
    }
    sf::Vector2f updatePlayer(bool hasfocused = true, bool hasclick = true);
    bool hasshoot = false;
    bool isDead();
private:
};
}

#endif // PLAYER_H
