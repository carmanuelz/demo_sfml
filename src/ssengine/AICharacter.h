#ifndef AICHARACTER_H
#define AICHARACTER_H

#include "character.h"

namespace sse
{
class AICharacter : public Character
{
    public:
        AICharacter(float inx, float iny, std::string inCode,  GameContext* incontext):Character(inx,iny,inCode,incontext)
        {
            AStartF = incontext->m_finder;
            TileSize = incontext->TileSize;
            preenemmyX = floor(inx/TileSize);
            preenemmyY = floor(inx/TileSize);
        }
        virtual ~AICharacter();
        void findto(float intargetx, float intargety);
        void setpathfinding(AStarFinder* AStarta, float intileSize);
        void update(sf::Time frameTime);
        void TestCollision();
        void updateFind();
        void setTarget(b2Body* inTarget);
        void setPatrol(float Ax, float Ay, float Bx, float By);
        void GotoPosition(float Ax, float Bx);
        void CastTarget(b2Body* inTarget, bool isdebug);
        void GoToOrigin();
        void follow(b2Body* inTarget);
        bool gotoflag = false;
        bool findflag = true;
    private:
        AStarFinder* AStartF;
        int preenemmyX = 0;
        int preenemmyY = 0;
        float TileSize = 0;
        float facVel = 0.18;
        bool patrolflag = false;
        sf::Vector2f findposition;
        std::vector<sf::Vector2i*> route;
    };
}

#endif // AICHARACTER_H
