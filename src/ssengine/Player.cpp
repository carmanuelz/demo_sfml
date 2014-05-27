#include "Player.h"

namespace sse
{

sf::Vector2f Player::updatePlayer(bool hasfocused, bool hasclick)
{
    float result = 0;
    if(hasfocused && !blockbehaviour)
    {
        Body->SetLinearVelocity(b2Vec2(0,0));
        vel = Body->GetLinearVelocity();
        bool isrun = false;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)||
                sf::Keyboard::isKeyPressed(sf::Keyboard::D)||
                sf::Keyboard::isKeyPressed(sf::Keyboard::W)||
                sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            setAnimCicle(2);
            isrun = true;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                vel.x = -SPEED;
                if(direction == 1)
                {
                    setRight();
                    animated.setReverse(true);
                }

                else
                {
                    setLeft();
                    animated.setReverse(false);
                }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                vel.x = SPEED;
                if(direction == 1)
                {
                    setRight();
                    animated.setReverse(false);
                }
                else
                {
                    setLeft();
                    animated.setReverse(true);
                }
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                vel.y = -SPEED;
                animated.setReverse(false);
                if(direction == 1)
                    setRight();
                else
                    setLeft();
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                animated.setReverse(false);
                vel.y = SPEED;
                if(direction == 1)
                    setRight();
                else
                    setLeft();
            }
        }

        if(!isrun)
        {
            setAnimCicle(1);
            if(direction == 1)
                setRight();
            else
                setLeft();
        }

        if(vel.x != 0 && vel.y!=0)
        {
            vel.x = vel.x*0.7;
            vel.y = vel.y*0.7;
        }

        weapon.play();
        if (!hasclick || !sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            weapon.stop();
        }

        else
        {
            if(Acumulator >= 0.2)
            {
                createBullet(bulletOrigin, bulletVU, angle);
                Acumulator = 0;
                soundshoot.play();
                hasshoot = true;
            }
        }
        if(Acumulator < 0.2 && hasshoot)
        {
            if(Acumulator < 0.05)
                result = pow(Acumulator,2)*600;
            else
                result = (Acumulator*-0.0167f+0.0033f)*600;

            context->impactview = bulletVU*-result;
        }
        else
        {
            hasshoot = false;
            context->impactview = sf::Vector2f(0,0);
        }

    }

    return sf::Vector2f(x,y);
}

bool Player::isDead()
{
    if(HP<=0)
        return true;
    return false;
}

}

