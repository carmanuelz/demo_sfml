#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <string>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Resources.hpp>

#include "debugrender.h"
#include "rubestuff/b2dJson.h"
#include "AStarFinder.h"
#include "loadconf/LuaScript.h"
#include "UserData.h"
#include "tween/TweenManager.h"
#include "KeyManager.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

namespace sse
{
class GameContext
{
public:
    GameContext(sf::RenderWindow* inrw);
    virtual ~GameContext();

    b2dJson* m_b2json = new b2dJson();
    sf::Vector2f mousePos;
    b2World* m_world;
    LuaScript* m_script;
    AStarFinder* m_finder;
    thor::ParticleSystem* m_psystem = &obj_psystem;
    sf::RenderWindow* m_rwindow;
    TweenManager* m_tweenmanager;
    sf::Vector2i m_screensize;
    sf::Vector2f impactview;
    float TileSize;
    bool LoadWorld(const char* filename, float tilesize, int width, int height);
    void DrawSysParticle();
    std::shared_ptr<sf::Texture> getPrtTexture(std::string key);
    thor::MultiResourceCache* cache;
protected:
private:
    DebugDraw* debugDraw;
    KeyManager<std::string, thor::ResourceKey<sf::Texture>> textureKeys;
    std::string errMsg;
    thor::ParticleSystem obj_psystem;
    void registerResurces();
};
}

#endif // GAMECONTEXT_H
