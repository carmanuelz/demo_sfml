#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include "GameContext.h"
#include "UserData.h"

namespace sse
{
class Bullet
{
public:
    Bullet(GameContext* incontext,sf::Vector2f origin, sf::Vector2f vel, float angle, std::string incode);
    virtual ~Bullet();
    void draw(sf::Time delta);
    int estado = 0;
private:
    std::string code;
    GameContext* context;
    b2BodyDef bulletBodyDef;
    b2Body* bulletB;
    b2Fixture* bulletFix;
    b2Filter filter;
    UserData* udBullet;
    std::shared_ptr<sf::Texture> texture;
    AnimatedSprite animated;
    Animation cile;
    sf::Sprite sprite;
    bool animatedflag;
    void loadFrames(Animation* anim,std::string luapath);
};
}

#endif // BULLET_H
