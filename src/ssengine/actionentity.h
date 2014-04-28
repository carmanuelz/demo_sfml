#ifndef ACTIONENTITY_H
#define ACTIONENTITY_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include "drawableentity.h"

namespace sse
{
    class actionentity : public drawableentity
    {
    public:
        actionentity(int inType, std::string inCode, b2World* inworld,LuaScript *inscript): drawableentity(2)
        {
            Code = inCode;
            Type = inType;
            world = inworld;
            x = inx;
            y = iny;
            script = inscript;
        }
    };

    class bullet : public actionentity
    {
    public:
        bullet(int inType, std::string inCode, b2World* inworld,LuaScript *inscript): actionentity(inType,inCode,inworld,inscript)
        {
            init();
        }
        init();
    };
}
