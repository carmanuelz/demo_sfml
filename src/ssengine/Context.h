#ifndef CONTEXT_H
#define CONTEXT_H

#include <Box2D/Box2D.h>

namespace sse
{
    class Context
    {
        b2World m_world;
        LuaScript* script;
        AStarFinder* AStarta;
        std::vector<b2Body*> RemoveList;
        std::vector<b2Body*> BulletList;
    };
}

#endif // CONTEXT_H
