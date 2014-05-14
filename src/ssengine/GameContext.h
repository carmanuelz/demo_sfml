#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <string>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include "../rubestuff/b2dJson.h"
#include "AStarFinder.h"
#include "loadconf/LuaScript.h"
#include "UserData.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

namespace sse
{
    class GameContext
    {
        public:
            GameContext();
            virtual ~GameContext();

            b2dJson* m_b2json = new b2dJson();
            std::vector<b2Body*> RemoveList;
            std::vector<b2Body*> BulletList;
            sf::Vector2f mousePos;
            b2World* m_world;
            LuaScript* m_script;
            AStarFinder* m_finder;
            thor::ParticleSystem* m_psystem = &obj_psystem;
            sf::RenderWindow* m_rwindow;
            float TileSize;
            bool LoadWorld(const char* filename);
            void Createfinder(float tilesize, int width, int height);
            void DrawSysParticle();
        protected:
        private:
            std::string errMsg;
            thor::ParticleSystem obj_psystem;
    };
}

#endif // GAMECONTEXT_H
