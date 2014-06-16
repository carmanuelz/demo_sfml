#include "Player.h"

namespace sse
{

void Player::update(sf::Time frameTime)
{
    TestCollision();
    Character::update(frameTime);
    if(Type == 1 && !isdeadflag)
    {
        if(HP <= 0)
        {
            vel = b2Vec2(0,0);
            setAnimCicle(5);
            animated.setReverse(false);
            animated.setLooped(false);
            isdeadflag = true;
            hasweapon = false;
            blockbehaviour = true;
        }
    }
}


sf::Vector2f Player::updateEventListener(bool hasfocused, bool hasclick)
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

        if (hasclick && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(Acumulator >= 0.2)
            {
                createBullet(bulletOrigin, bulletVU, angle);
                Acumulator = 0;
                weapon.play();
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

void Player::TestCollision()
{
    for(auto k = CollisionList.cbegin() ; k != CollisionList.cend() ; k++ )
    {
        b2Fixture* f = *k;
        sse::UserData* userdata = static_cast<sse::UserData*>(f->GetUserData());
        switch (userdata->tipo)
        {
        case objectType::obj_typeBullet:
            Bullet* bullet = static_cast<sse::Bullet*>(f->GetBody()->GetUserData());
            if(bullet->estado == 0)
            {
                context->m_psystem->addEmitter(BloodEmitter(sf::Vector2f(x,y)), sf::seconds(0.1f));
                bullet->estado = 1;
                takeDamage(5);
                if(Type == objectType::obj_typeEnemy)
                    if(Target == 0)
                    {
                        setAnimCicle(2);
                        Character* player = static_cast<sse::Character*>(bullet->fromcharacter);
                        Target = player->Body;
                    }
            }
            break;
        }

        if(HP<15 && finished == false)
        {
            context->m_tweenmanager->add(&tween);
            finished = true;
        }
    }
}


}

