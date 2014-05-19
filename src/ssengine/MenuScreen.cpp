#include "MenuScreen.h"

MenuScreen::MenuScreen(sf::RenderWindow* rw):renderwindow(rw)
{
    screnSize = sf::Vector2i((int)renderwindow->getSize().x,(int)renderwindow->getSize().y);
    pointerT.loadFromFile("assets/pointer.png");
    pointerS.setTexture(pointerT);

    m_label = sfg::Label::Create( "Menu" );
    // Create a simple button and connect the click signal.
    auto star_button = sfg::Button::Create( "Start Game" );
    star_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &MenuScreen::OnButtonClick, this ) );
    auto exit_button = sfg::Button::Create( "Exit" );
    exit_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &MenuScreen::ExitClick, this ) );

    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    box->Pack( m_label );
    box->Pack( star_button, false );
    box->Pack( exit_button, false );

    // Create a window and add the box layouter to it. Also set the window's title.
    window->SetStyle( window->GetStyle() ^ sfg::Window::TITLEBAR );
    window->SetStyle( window->GetStyle() ^ sfg::Window::BACKGROUND);
    window->SetStyle( window->GetStyle() ^ sfg::Window::RESIZE );
    window->SetPosition( sf::Vector2f( 350.f, 190.f ) );
    window->Add( box );

    desktop.LoadThemeFromFile( "assets/example.theme" );
    desktop.Add( window );
}

MenuScreen::~MenuScreen()
{
    //dtor
}

int MenuScreen::Run()
{
    while(renderwindow->isOpen())
    {
        if(gotoWin != 0)
            return gotoWin;
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
    std::cout<<"terminado"<<std::endl;
    return -1;
}

void MenuScreen::OnButtonClick()
{
    window->Show(false);
    gotoWin = 1;
}
void MenuScreen::ExitClick()
{
    window->Show(false);
    gotoWin = -1;
}
