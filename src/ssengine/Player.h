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
        moveimpactview = sf::Vector2f(0,0);
    }
    sf::Vector2f updatePlayer(bool hasfocused, bool hasclick);
    sf::Vector2f moveimpactview;
    bool hasshoot = false;
private:
};
}

#endif // PLAYER_H
