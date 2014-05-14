#ifndef DRAWABLEENTITY_H
#define DRAWABLEENTITY_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "GameContext.h"
#include "AnimatedSprite.hpp"

namespace sse
{
    class drawableentity
    {
    public:
        drawableentity(int inTypeDraw,GameContext* incontext):TypeDraw(inTypeDraw),context(incontext){}
        int TypeDraw;
        AnimatedSprite animated;
        AnimatedSprite weapon;
        sf::Sprite sprite;
        GameContext* context;
        bool hasweapon = false;
        bool before = false;
        float x = 0;
        float y = 0;
        void draw()
        {
            switch(TypeDraw)
            {
            case 1:
                drawCharacter();
                break;
            }
        }
    private:
        void drawCharacter()
        {
            if(before)
            {
                if(hasweapon)
                    context->m_rwindow->draw(weapon);
                context->m_rwindow->draw(animated);
            }
            else
            {
                context->m_rwindow->draw(animated);
                if(hasweapon)
                    context->m_rwindow->draw(weapon);
            }
        }

    };
}

#endif
