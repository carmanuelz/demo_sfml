#include <math.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ssengine/Level1.h"

int main()
{
    std::vector<Screen*> Screens;
    int screen = 0;

    sf::VideoMode videomode(800, 600, 32);
    sf::RenderWindow renderWindow(videomode, "Test"/*,sf::Style::Fullscreen*/);
    renderWindow.setVerticalSyncEnabled(true);
    renderWindow.setFramerateLimit(60);
    renderWindow.setMouseCursorVisible(false);

    Level1* s0 = new Level1(&renderWindow);
    Screens.push_back(s0);

    //Main loop
    while (screen >= 0)
    {
        screen = Screens[screen]->Run();
    }

    return 0;
}
