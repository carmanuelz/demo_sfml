#include "LoaderScreen.h"

int LoaderScreen::Run()
{
    while(loaderFlag)
    {
        sf::Event event;
        while(renderwindow->pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    return -1;
                }
            }
        }
        sf::Time frameTime = frameClock.restart();
        animated.update(frameTime);
        renderwindow->clear();
        renderwindow->draw(backgroundS);
        renderwindow->draw(animated);
        renderwindow->display();
    }
    std::cout<<"terminado"<<std::endl;
    return -1;
}
