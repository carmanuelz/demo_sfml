#include "AICharacter.h"

namespace sse
{
AICharacter::~AICharacter()
{
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

    std::vector<sf::Vector2i*> route;
    if(AStartF->isWalkableAt(startX,startY) && AStartF->isWalkableAt(endX,endY))
    {
        route = AStartF->findPath(startX,startY,endX,endY);
        int len = route.size();
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
        }
    }
}

sf::Vector2f AICharacter::CastTarget(b2Body* inTarget)
{
    b2Vec2 p1 = inTarget->GetPosition();
    b2Vec2 p3 = Body->GetPosition();
    b2Vec2 dif2 = p1 - p3;
    float module2 = sqrt(pow(dif2.x,2)+pow(dif2.y,2));
    b2Vec2 p4 = p3 + b2Vec2(dif2.x/module2*6,dif2.y/module2*6);
    sf::Vector2f targetbehaviour;
        if(isright)
            targetbehaviour = sf::Vector2f(p3.x*PPM+50, p3.y*PPM);
        else
            targetbehaviour = sf::Vector2f(p3.x*PPM-50, p3.y*PPM);
    sse::MyRayCastCallback RayCastCallback2;
    context->m_world->RayCast(&RayCastCallback2, p3 , p4);
    if ( RayCastCallback2.m_fixture )
    {
        sse::UserData* userdataA = static_cast<sse::UserData*>(RayCastCallback2.m_fixture->GetUserData());
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
        p4 = b2Vec2(RayCastCallback2.m_point.x, RayCastCallback2.m_point.y);

        if(!gotoflag)
            targetbehaviour = sf::Vector2f(p4.x*PPM, p4.y*PPM);
    }
    else
        findflag = true;
    if(hasweapon)
    {
        updatebehaviour(targetbehaviour.x, targetbehaviour.y);
        if(!shootbusy && Target != 0)
        {
            if(!findflag)
            {
                currenttimefind = 0;
                createBullet(bulletOrigin, bulletVU, angle);
                //std::cout<<"enemy: "<<p3.x*PPM<<"-"<<p3.y*PPM<<"//"<<bulletOrigin.x<<"-"<<bulletOrigin.y<<"target:"<<targetbehaviour.x<<"-"<<targetbehaviour.y<<std::endl;
                weapon.play();
                shootbusy = true;
            }
        }
    }
    return sf::Vector2f(p4.x*PPM, p4.y*PPM);
}

}
