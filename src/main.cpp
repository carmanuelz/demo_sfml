#include <math.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ssengine/Level1.h"
#include "ssengine/LoaderScreen.h"

void RenderLoader(LoaderScreen* loader)
{
    int screen = 1;
    while(screen>0)
        screen = loader->Run();
}

int main()
{
    int screen = 0;
    sf::VideoMode videomode(800, 600, 32);
    std::vector<Screen*> Screens;
    sf::RenderWindow renderWindow(videomode, "Test"/*,sf::Style::Fullscreen*/);
    renderWindow.setVerticalSyncEnabled(true);
    renderWindow.setFramerateLimit(60);
    renderWindow.setMouseCursorVisible(false);
    renderWindow.setActive(false);
    LoaderScreen loader(&renderWindow);
    sf::Thread thread(std::bind(&RenderLoader,&loader));

    thread.launch();

    renderWindow.setActive(false);
    Screens.push_back(new Level1(&renderWindow));

    loader.loaderFlag = false;
    //Main loop
    while (screen >= 0)
    {
        screen = Screens[screen]->Run();
    }

    return 0;
}
