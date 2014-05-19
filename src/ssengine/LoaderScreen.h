#ifndef LOADERSCREEN_H
#define LOADERSCREEN_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "AnimatedSprite.hpp"
#include "Screen.h"
class LoaderScreen : public Screen
{
public:
    LoaderScreen(sf::RenderWindow* rw):renderwindow(rw)
    {
        textureanimation.loadFromFile("assets/enemmy.png");
        backgroundT.loadFromFile("assets/loadingground.png");
        backgroundS.setTexture(backgroundT);
        cicle.setSpriteSheet(textureanimation);
        for(int i = 0 ; i < 4 ; i++)
        {
            int val = i*70;
            cicle.addFrame(sf::IntRect(val, 0, 70, 40));
        }
        animated = AnimatedSprite(sf::seconds(0.1f), true, false);
        animated.setLooped(true);
        animated.setPosition(365,280);
        animated.play(cicle);
    }
    bool loaderFlag = true;
    virtual int Run();
private:
    sf::RenderWindow* renderwindow;
    sf::Texture textureanimation;
    sf::Texture backgroundT;
    sf::Sprite backgroundS;
    sf::Clock frameClock;
    AnimatedSprite animated;
    Animation cicle;
};

#endif // LOADERSCREEN_H

