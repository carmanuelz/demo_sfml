#include "InstruccionesScreen.h"

InstruccionesScreen::InstruccionesScreen(sf::RenderWindow* rw):renderwindow(rw)
{
    screnSize = sf::Vector2i((int)renderwindow->getSize().x,(int)renderwindow->getSize().y);
    pointerT.loadFromFile("assets/pointer.png");
    pointerS.setTexture(pointerT);
    // Create a simple button and connect the click signal.
    auto return_button = sfg::Button::Create( " Volver " );
    return_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &InstruccionesScreen::ReturnClick, this ) );


    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    box->Pack( sfg::Label::Create( "Instrucciones:" ), false );
    box->Pack( sfg::Label::Create( " " ), false );
    box->Pack( sfg::Label::Create( "Movimiento: A/S/W/D \n"
                                  "Objetivo: Puntero \n"
                                  "Menu: Esc"), false );
    box->Pack( sfg::Label::Create( " " ), false );
    box->Pack( return_button, false );

    window->Add( box );
    // Create a window and add the box layouter to it. Also set the window's title.
    window->SetStyle( window->GetStyle() ^ sfg::Window::TITLEBAR );
    window->SetStyle( window->GetStyle() ^ sfg::Window::BACKGROUND);
    window->SetStyle( window->GetStyle() ^ sfg::Window::RESIZE );

    desktop.LoadThemeFromFile( "assets/example.theme" );
    desktop.Add( window );

    sf::FloatRect allocation = window->GetAllocation();
    window->SetPosition( sf::Vector2f( screnSize.x - allocation.width, screnSize.y-allocation.height )/2.0f );
}

InstruccionesScreen::~InstruccionesScreen()
{
    //dtor
}

int InstruccionesScreen::Run()
{
    window->Show(true);
    while(renderwindow->isOpen())
    {
        if(gotoWin > -2)
        {
            int rvalue = gotoWin;
            gotoWin = -2;
            return rvalue;
        }

        sf::Event event;
        while(renderwindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                return (-1);
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    return -1;
                }
            }
            desktop.HandleEvent( event );
        }
        sf::Vector2i winmouseposition = sf::Mouse::getPosition(*renderwindow);
        sf::View view = renderwindow->getView();
        sf::Vector2f mousePos(winmouseposition.x + view.getCenter().x - screnSize.x/2 ,winmouseposition.y + view.getCenter().y - screnSize.y/2);
        sf::Time frameTime = frameClock.restart();
        pointerS.setPosition(mousePos);
        renderwindow->clear();
        desktop.Update( frameTime.asSeconds() );
        m_sfgui.Display(*(renderwindow));
        renderwindow->draw(pointerS);
        renderwindow->display();
    }
    return -1;
}

void InstruccionesScreen::ReturnClick()
{
    window->Show(false);
    gotoWin = 0;
}
