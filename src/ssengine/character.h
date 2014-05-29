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

#include "drawableentity.h"
#include "AStarFinder.h"
#include "particlesys.h"
#include "Bullet.h"

#include "AnimatedAccessor.h"

#define M_PI 3.14159265358979323846
#define SPEED 0.3
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PPM 64.0f               //PIXELS POR METRO
#define MPP (1.0f/PPM)


namespace sse
{
namespace objectType
{
const static int obj_typeBullet = 3;
const static int obj_typePlayer = 1;
const static int obj_typeEnemy = 4;
}

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
        UserData* userdata = static_cast<sse::UserData*>(fixture->GetUserData());
        if(userdata->tipo == 3)
        {
            return 1.0f;
        }
        if(fraction <= 1.0f)
        {
            m_fixture = fixture;
            m_point = point;
            m_normal = normal;
            m_fraction = fraction;
        }
        return 1.0f;
    }

    b2Fixture* m_fixture;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float32 m_fraction;
};
class Character : public drawableentity
{
public:
    Character(float inx, float iny, std::string inCode, GameContext* incontext) : drawableentity(1,incontext)
    {
        Code = inCode;
        x = inx;
        y = iny;
        Origin = sf::Vector2f(inx,iny);
        init();
    }
    virtual ~Character();

    b2Vec2 vel;
    int Type;
    b2Body* Body;
    void update(sf::Time frameTime);
    std::string Code = "";
    void setAnimCicle(int codecicle);
    void updatebehaviour(float TargetX,float TargetY);
    int direction = 1;
    sf::Vector2f CgunOffset;
    float angle = 0;
    sf::Vector2f bulletVU;
    sf::Vector2f bulletOrigin;
    void setweapon(std::string weaponcode);
    float Acumulator = 0;
    sf::Sprite bulletS;
    sf::Sound soundshoot;
    bool isbusy = false;
    bool shootbusy = false;
    void takeDamage(float inDamage);
    float Hit();
    float getHP();
    std::vector<b2Fixture*> CollisionList;
    void TestCollision();
    void addCollisionList(b2Fixture*f);
    void removeCollisionList(b2Fixture*f);
    void setBulletList(std::vector<Bullet*>* inBulletList){BulletList = inBulletList;}
    b2Body* Target = 0;
    bool blockbehaviour = false;
protected:
    b2Body* createBullet(sf::Vector2f origin, sf::Vector2f bvel,float angle);
    float offsetAnimXR = 0;
    float offsetAnimXL = 0;
    float offsetAnimYR = 0;
    float offsetAnimYL = 0;
    float currenttimefind = 0;
    float limittimefind = 4;
    float HP = 0;
    float FullHP;
    float Damage = 0;
    void setLeft();
    void setRight();
    sf::Vector2f Origin;
    std::vector<Bullet*>* BulletList;
    bool isright = false;
private:
    bool isdeadflag = false;
    int ofsetanimx = 0;
    Animation Cstartcile;
    Animation Cruncicle;
    Animation Cidlecile;
    Animation Cattackcicle;
    Animation ChideIcle;
    Animation Cdeadcile;
    Animation waposhootcicle;
    sf::Texture Cspritesheet;
    sf::Texture weaponspritesheet;
    std::shared_ptr<sf::Texture> wspritesheet;
    sf::SoundBuffer buffershoot;
    void init();
    b2Body* createBody(float x, float y);
    void loadFrames(Animation* anim,std::string luapath);
    AnimatedAccessor* animaccess;
    sse::Tween tween;
    sse::tweenOptions* options;
};
/*
class Mob : public AICharacter
{
private:
    //Mob():AICharacter(){}
    float HP;
public:
};*/
}

#endif
