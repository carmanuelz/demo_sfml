#include <math.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ssengine/Level.h"
#include "ssengine/MenuScreen.h"
#include "ssengine/LoaderScreen.h"
#include "ssengine/InstruccionesScreen.h"
#include "icon.h"

int main()
{
    int screen = 0;
    sf::VideoMode videomode(1024, 600, 32);
    std::vector<Screen*> Screens;
    sf::RenderWindow renderWindow(videomode, "Test",sf::Style::None);

    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                  << mode.width << "x" << mode.height << " - "
                  << mode.bitsPerPixel << " bpp" << std::endl;
    }
    // Create a window with the same pixel depth as the desktop
    //renderWindow.setFramerateLimit(60);
    renderWindow.setVerticalSyncEnabled(true);
    renderWindow.setMouseCursorVisible(false);
    renderWindow.setTitle("April's Quest");
    renderWindow.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
    thor::MultiResourceCache cache;
    sf::Texture backgroundT;
    sf::Sprite backgroundS;
    backgroundT.loadFromFile("assets/images/others/loadingground.png");
    backgroundS.setTexture(backgroundT);
    renderWindow.clear();
    renderWindow.draw(backgroundS);
    renderWindow.display();
    Screens.push_back(new MenuScreen(&renderWindow));
    Screens.push_back(new Level(&renderWindow,&cache,"nivel_1_1"));
    Screens.push_back(new InstruccionesScreen(&renderWindow));
    //loader.loaderFlag = false;
    //Main loop
    //renderWindow.setActive(true);
    while (screen >= 0)
    {
        screen = Screens[screen]->Run();
    }

    return 0;
}
