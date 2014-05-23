#ifndef INSTRUCCIONESSCREEN_H
#define INSTRUCCIONESSCREEN_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFGUI/SFGUI.hpp>
#include "Screen.h"

class InstruccionesScreen : public Screen
{
    public:
        InstruccionesScreen(sf::RenderWindow* rw);
        virtual ~InstruccionesScreen();
        virtual int Run();
    protected:
    private:
        sf::RenderWindow* renderwindow;
        void ReturnClick();

        sfg::SFGUI m_sfgui;
        sfg::Label::Ptr m_label;
        sfg::Window::Ptr window = sfg::Window::Create();
        sfg::Desktop desktop;

        sf::Clock frameClock;
        sf::Sprite pointerS;
        sf::Texture pointerT;
        sf::Vector2i screnSize;
        int gotoWin = -2;
};

#endif // INSTRUCCIONESSCREEN_H
