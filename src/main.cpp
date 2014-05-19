#include <math.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ssengine/Level1.h"
#include "ssengine/MenuScreen.h"
#include "ssengine/LoaderScreen.h"
#include "icon.h"

void RenderLoader(LoaderScreen* loader, sf::RenderWindow* rw)
{
    sf::Context context;
    int screen = 0;
    while(screen>=0)
        screen = loader->Run();
    std::cout<<"listo";
    //rw->setActive(false);
}

int main()
{
    int screen = 0;
    sf::VideoMode videomode(800, 600, 32);
    std::vector<Screen*> Screens;
    sf::RenderWindow renderWindow(videomode, "Test"/*,sf::Style::Fullscreen*/);
    //renderWindow.setVerticalSyncEnabled(true);
    renderWindow.setFramerateLimit(60);
    renderWindow.setMouseCursorVisible(false);
    renderWindow.setTitle("April's Quest");
    renderWindow.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);
    //renderWindow.setActive(false);
    //LoaderScreen loader(&renderWindow);
    //sf::Thread thread(std::bind(&RenderLoader,&loader,&renderWindow));
    //thread.launch();
    sf::Texture backgroundT;
    sf::Sprite backgroundS;
    backgroundT.loadFromFile("assets/loadingground.png");
    backgroundS.setTexture(backgroundT);
    renderWindow.clear();
    renderWindow.draw(backgroundS);
    renderWindow.display();
    Screens.push_back(new MenuScreen(&renderWindow));
    Screens.push_back(new Level1(&renderWindow));
    //loader.loaderFlag = false;
    //Main loop
    //renderWindow.setActive(true);
    while (screen >= 0)
    {
        screen = Screens[screen]->Run();
    }

    return 0;
}
