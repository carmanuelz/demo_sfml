#include "AICharacter.h"

namespace sse
{
AICharacter::~AICharacter()
{
}

void AICharacter::update(sf::Time frameTime)
{
    updateFind();
    TestCollision();
    Character::update(frameTime);
}

void AICharacter::updateFind()
{
    if(Target != 0)
    {
        if(currenttimefind > limittimefind)
        {
            Target = 0;
            GotoPosition(Origin.x, Origin.y);
        }
        else
        {
            if(findflag)
            {
                if(hasweapon)
                    setAnimCicle(2);
                b2Vec2 destination = Target->GetPosition();
                destination.x*=PPM;
                destination.y*=PPM;
                findto(destination.x,destination.y);
            }
            else
            {
                vel = b2Vec2(0,0);
                setAnimCicle(1);
            }
        }
    }
    else
    {
        if(gotoflag)
        {
            if(floor(x/TileSize)==floor(findposition.x/TileSize) && floor(y/TileSize)==floor(findposition.y/TileSize))
            {
                vel = b2Vec2(0,0);
                setAnimCicle(1);
                gotoflag = false;
                currenttimefind = 0;
            }
            else
                findto(findposition.x,findposition.y);
        }
    }
}

void AICharacter::GotoPosition(float Ax, float Bx)
{
    gotoflag = true;
    findposition = sf::Vector2f(Ax,Bx);
    setAnimCicle(2);
}

void AICharacter::GoToOrigin()
{
    Target = 0;
    gotoflag = true;
    findposition = sf::Vector2f(Origin.x,Origin.y);
    setAnimCicle(2);
}

void AICharacter::setTarget(b2Body* inTarget)
{
    Target = inTarget;
}

void AICharacter::findto(float tox, float toy)
{
    int endX = floor(tox/TileSize);
    int endY = floor(toy/TileSize);
    float floatX = x/TileSize;
    float floatY = y/TileSize;
    int startX;
    int startY;
    if((floatX - preenemmyX)>1.5f || (preenemmyX - floatX)>0.5f)
    {
        startX = floor(floatX);
        preenemmyX = startX;
    }
    else
    {
        startX = preenemmyX;
    }

    if((floatY - preenemmyY)>1.5f || (preenemmyY - floatY)>0.5f)
    {
        startY = floor(floatY);
        preenemmyY = startY;
    }
    else
    {
        startY = preenemmyY;
    }

    if(AStartF->isWalkableAt(startX,startY) && AStartF->isWalkableAt(endX,endY))
    {
        route = AStartF->findPath(startX,startY,endX,endY);
        /*int len = route.size();
        if(len>1)
        {
            sf::Vector2f difstep((route[len-2]->x - startX)*TileSize, (route[len-2]->y - startY)*TileSize);

            float modenemy = sqrt(pow(difstep.x,2)+pow(difstep.y,2));

            if(modenemy != 0)
            {
                sf::Vector2f enemmyVU((difstep.x)/modenemy,(difstep.y)/modenemy);
                b2Vec2 enemyVel(enemmyVU.x*facVel,enemmyVU.y*facVel);
                vel = enemyVel;
                if(enemyVel.x > 0)
                    setRight();
                else
                {
                    if(enemyVel.x < 0)
                        setLeft();
                    else
                    {
                        if(tox < x)
                            setLeft();

                        else
                            setRight();

                    }
                }
            }
        }*/
    }
}

void AICharacter::CastTarget(b2Body* inTarget, bool isdebug)
{
    b2Vec2 targetpos = inTarget->GetPosition();
    b2Vec2 originpos = Body->GetPosition();

    b2Vec2 dif = targetpos - originpos;
    float module = sqrt(pow(dif.x,2)+pow(dif.y,2));
    b2Vec2 direction = b2Vec2(dif.x/module,dif.y/module);

    b2Vec2 originA = originpos + b2Vec2(direction.y*0.1f,-direction.x*0.1f);
    b2Vec2 originB = originpos + b2Vec2(-direction.y*0.1f,direction.x*0.1f);

    b2Vec2 FinishA = originA + b2Vec2(direction.x*6,direction.y*6);
    b2Vec2 FinishB = originB + b2Vec2(direction.x*6,direction.y*6);
    b2Vec2 FinishC = originpos + b2Vec2(direction.x*6,direction.y*6);

    sf::Vector2f targetbehaviour;
        if(isright)
            targetbehaviour = sf::Vector2f(originpos.x*PPM+50, originpos.y*PPM);
        else
            targetbehaviour = sf::Vector2f(originpos.x*PPM-50, originpos.y*PPM);

    sse::MyRayCastCallback RayCastCallbackA;
    sse::MyRayCastCallback RayCastCallbackB;
    sse::MyRayCastCallback RayCastCallbackC;

    if(hasweapon)
    {
        context->m_world->RayCast(&RayCastCallbackA, originA , FinishA);
        context->m_world->RayCast(&RayCastCallbackB, originB , FinishB);
        if ( RayCastCallbackA.m_fixture == RayCastCallbackB.m_fixture && RayCastCallbackA.m_fixture)
        {
            sse::UserData* userdataA = static_cast<sse::UserData*>(RayCastCallbackA.m_fixture->GetUserData());
            if(Target == 0 && !gotoflag)
            {
                if(userdataA->tipo == 1)
                {
                    setAnimCicle(2);
                    setTarget(inTarget);
                }
            }
            if(hasweapon)
            {
                if(userdataA->tipo == 1)
                    findflag = false;
                else
                    findflag = true;
            }
            FinishA = b2Vec2(RayCastCallbackA.m_point.x, RayCastCallbackA.m_point.y);
            FinishB = b2Vec2(RayCastCallbackB.m_point.x, RayCastCallbackB.m_point.y);

            if(!gotoflag)
                targetbehaviour = sf::Vector2f(FinishC.x*PPM, FinishC.y*PPM);
        }
        else
            findflag = true;
    }
    else
    {
        context->m_world->RayCast(&RayCastCallbackC, originpos , FinishC);
        if ( RayCastCallbackC.m_fixture)
        {
            sse::UserData* userdataA = static_cast<sse::UserData*>(RayCastCallbackC.m_fixture->GetUserData());
            if(Target == 0 && !gotoflag)
            {
                if(userdataA->tipo == 1)
                {
                    setAnimCicle(2);
                    setTarget(inTarget);
                }
            }
            FinishC = b2Vec2(RayCastCallbackC.m_point.x, RayCastCallbackC.m_point.y);
        }
    }

    if(hasweapon)
    {
        updatebehaviour(targetbehaviour.x, targetbehaviour.y);
        if(!shootbusy && Target != 0)
        {
            if(!findflag)
            {
                createBullet(bulletOrigin, bulletVU, angle);
                weapon.play();
                soundshoot.play();
                shootbusy = true;
                currenttimefind = 0;
                Acumulator = 0;
            }
        }
    }

    if(isdebug)
    {
        if(hasweapon)
        {
                    sf::Vertex lineA[] =
                    {
                        sf::Vertex(sf::Vector2f(originA.x*PPM, originA.y*PPM)),
                        sf::Vertex(sf::Vector2f(FinishA.x*PPM, FinishA.y*PPM))
                    };
            sf::Vertex lineB[] =
                    {
                        sf::Vertex(sf::Vector2f(originB.x*PPM, originB.y*PPM)),
                        sf::Vertex(sf::Vector2f(FinishB.x*PPM, FinishB.y*PPM))
                    };

        context->m_rwindow->draw(lineA, 2, sf::Lines);
        context->m_rwindow->draw(lineB, 2, sf::Lines);
        }
        else
        {
            sf::Vertex lineC[] =
            {
                sf::Vertex(sf::Vector2f(originpos.x*PPM, originpos.y*PPM)),
                sf::Vertex(sf::Vector2f(FinishC.x*PPM, FinishC.y*PPM))
            };
            context->m_rwindow->draw(lineC, 2, sf::Lines);
        }
        if(Target != 0)
        {
            sf::VertexArray lines(sf::LinesStrip, route.size());
            for(int i = 0; i < route.size(); i++)
            {
                lines[i].position = sf::Vector2f(route[i]->x*TileSize, route[i]->y*TileSize);
                lines[i].color = sf::Color::Red;
            }
            context->m_rwindow->draw(lines);
        }
    }
}

void AICharacter::TestCollision()
{
    for(auto k = CollisionList.cbegin() ; k != CollisionList.cend() ; k++ )
    {
        b2Fixture* f = *k;
        sse::UserData* userdata = static_cast<sse::UserData*>(f->GetUserData());
        switch (userdata->tipo)
        {
        case objectType::obj_typePlayer:
            if(!isbusy)
            {
                if(!hasweapon)
                {
                    animated.PrepareTimeLine();
                    animated.PushTransition(new Transition(1,Cattackcicle ,0,0,0,0));
                    animated.PushTransition(new Transition(1,Cruncicle,0,0,0,0));
                    animated.StartTimeLine();
                    isbusy = true;
                    b2Vec2 pos = f->GetBody()->GetPosition();
                    context->m_psystem->addEmitter(BloodEmitter(sf::Vector2f(pos.x*PPM,pos.y*PPM)), sf::seconds(0.1f));
                    Character* player = static_cast<sse::Character*>(f->GetBody()->GetUserData());
                    player->takeDamage(Hit());
                }
            }
            break;
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
        if(HP <= 0 && Type != 1)
            needremove = true;
    }
}

}
