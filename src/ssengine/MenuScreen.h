#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFGUI/SFGUI.hpp>
#include "Screen.h"


class MenuScreen : public Screen
{
    public:
        MenuScreen(sf::RenderWindow* rw);
        virtual ~MenuScreen();
        virtual int Run();
    protected:
    private:
        sf::RenderWindow* renderwindow;
        void OnButtonClick();
        void ExitClick();

        sfg::SFGUI m_sfgui;
        sfg::Label::Ptr m_label;
        sfg::Window::Ptr window = sfg::Window::Create();
        sfg::Desktop desktop;

        sf::Clock frameClock;
        sf::Sprite pointerS;
        sf::Texture pointerT;
        sf::Vector2i screnSize;
        int gotoWin = 0;
};

#endif // MENUSCREEN_H
