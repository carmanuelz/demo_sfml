#ifndef DRAWABLEENTITY_H
#define DRAWABLEENTITY_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace sse
{
    class drawableentity
    {
    public:
        drawableentity(int inTypeDraw):TypeDraw(inTypeDraw){}
        int TypeDraw;
        AnimatedSprite animated;
        AnimatedSprite weapon;
        sf::Sprite sprite;
        bool hasweapon = false;
        bool before = false;
        float x = 0;
        float y = 0;
        void setRenderWindows(sf::RenderWindow* inRW)
        {
            RW = inRW;
        }
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
        sf::RenderWindow* RW;
        void drawCharacter()
        {
            if(before)
            {
                if(hasweapon)
                    RW->draw(weapon);
                RW->draw(animated);
            }
            else
            {
                RW->draw(animated);
                if(hasweapon)
                    RW->draw(weapon);
            }
        }

    };
}

#endif
