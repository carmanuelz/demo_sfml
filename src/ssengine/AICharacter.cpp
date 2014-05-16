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
            b2Vec2 destination = Target->GetPosition();
            destination.x*=PPM;
            destination.y*=PPM;
            findto(destination.x,destination.y);
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
    if(abs(endX - floatX) <= 1)
        startX = floor(floatX);
    else
    {
        if((floatX - preenemmyX)>1.5f || (preenemmyX - floatX)>0.5f)
        {
            startX = floor(floatX);
            preenemmyX = startX;
        }
        else
        {
            startX = preenemmyX;
        }
    }

    if(abs(endY - floatY) <= 1)
        startY = floor(floatY);
    else
    {
        if((floatY - preenemmyY)>1.5f || (preenemmyY - floatY)>0.5f)
        {
            startY = floor(floatY);
            preenemmyY = startY;
        }
        else
        {
            startY = preenemmyY;
        }
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

}
