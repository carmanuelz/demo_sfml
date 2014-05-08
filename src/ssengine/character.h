#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>

#include "AnimatedSprite.hpp"
#include "drawableentity.h"
#include "AStarFinder.h"
#include "loadconf/LuaScript.h"
#include "particlesys.h"

#include "../AnimatedAccessor.h"

#define M_PI 3.14159265358979323846
#define SPEED 0.3
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PPM 64.0f               //PIXELS POR METRO
#define MPP (1.0f/PPM)


namespace sse
{
    class MyRayCastCallback : public b2RayCastCallback
    {
        public:
            MyRayCastCallback()
            {
                m_fixture = NULL;
                m_fraction = 2.0f;
            }
            float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
            {
                m_fixture = fixture;
                m_point = point;
                m_normal = normal;
                //m_fraction = fraction;

                return fraction;
            }

             b2Fixture* m_fixture;
             b2Vec2 m_point;
             b2Vec2 m_normal;
             float32 m_fraction;
    };

    class UserData
    {
        public:
            int tipo;
            int estado;
    };

    class Character : public drawableentity
    {
    public:
        Character(float inx, float iny, int inType, std::string inCode, b2World* inworld,LuaScript *inscript) : drawableentity(1)
        {
            Code = inCode;
            Type = inType;
            world = inworld;
            x = inx;
            y = iny;
            script = inscript;
            init();
        }
        b2Vec2 vel;
        LuaScript *script;
        int Type;
        void update(sf::Time frameTime);
        std::string Code = "";
        void setLeft();
        void setRight();
        b2Body* Body;
        float offsetAnimXR = 0;
        float offsetAnimXL = 0;
        float offsetAnimYR = 0;
        float offsetAnimYL = 0;
        void setAnimCicle(int codecicle);
        sf::Vector2f updatebehaviour(float TargetX,float TargetY);
        int direction = 1;
        sf::Vector2f CgunOffset;
        float angle = 0;
        sf::Vector2f bulletVU;
        sf::Vector2f bulletOrigin;
        void setweapon(std::string weaponcode);
        float Acumulator = 0;
        sf::Sprite bulletS;
        sf::Sound soundshoot;
        b2World* world;
        thor::ParticleSystem* system;
        bool isbusy = false;
        b2Body* createBullet(sf::Vector2f origin, sf::Vector2f vel,float angle);
        void setBulletList(std::vector<b2Body*> *inBulletList)
        {
            BulletList = inBulletList;
        }
        void setParticleSystem(thor::ParticleSystem* insystem)
        {
            system = insystem;
        }
        void collisionCB(b2Fixture* inFixture);

        void takeDamage(float inDamage);
        float Hit();
        float getHP();

        std::vector<b2Body*> *BulletList;
    private:
        int ofsetanimx = 0;
        Animation Cstartcile;
        Animation Cruncicle;
        Animation Cidleicle;
        Animation Cattackcicle;
        Animation ChideIcle;
        Animation waposhootcicle;
        float HP = 0;
        float Damage = 0;
        sf::Texture Cspritesheet;
        sf::Texture weaponspritesheet;
        sf::SoundBuffer buffershoot;
        sf::Texture bulletT;
        void init();
        b2Body* createBody(float x, float y);
        void loadFrames(Animation* anim,std::string luapath);
    };

    class AICharacter : public Character
    {
    public:
        AICharacter(float inx, float iny, int inType, std::string inCode, b2World* inworld,LuaScript *inscript):Character(inx,iny,inType,inCode,inworld,inscript)
        {
        }
        void findto(float intargetx, float intargety);
        void setpathfinding(AStarFinder* AStarta, float intileSize);
        void update(sf::Time frameTim);
        void updateFind();
        void setTarget(b2Body* inTarget);
        void setPatrol(float Ax, float Ay, float Bx, float By);
        void GotoPosition(float Ax, float Bx);
        void follow(b2Body* inTarget);
        void setRayCastCallbackList();
        sf::Vertex debugRayCast[2];
        b2Body* Target = 0;
    private:
        AStarFinder* AStartF;
        int preenemmyX = 0;
        int preenemmyY = 0;
        float TileSize = 0;
        float facVel = 0.25;
        bool isPatrol = false;
    };

    class Mob : public AICharacter
    {
    private:
        //Mob():AICharacter(){}
        float HP;
    public:
    };

    class Player : public Character
    {
    public:
        Player(float inx, float iny, int inType, std::string inCode, b2World* inworld,LuaScript *inscript):Character(inx,iny,inType,inCode,inworld,inscript)
        {
            moveimpactview = sf::Vector2f(0,0);
        }
        void update(sf::Time frameTim);
        sf::Vector2f updatePlayer(bool hasfocused, bool hasclick);
        sf::Vector2f moveimpactview;
        bool hasshoot = false;
    private:
    };

    class Weapon
    {
    public:
    };
}

#endif
