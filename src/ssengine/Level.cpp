
#include "Level.h"

namespace sse
{
bool Level::compareByY(const sse::drawableentity* a, const sse::drawableentity* b)
{
    return a->y < b->y;
}

Level::Level(sf::RenderWindow* rw, thor::MultiResourceCache* incache, std::string inlevel):name_level(inlevel)
{
    context = new sse::GameContext(rw);
    context->m_psystem->addAffector(BloodkAffector());
    context->m_tweenmanager = new sse::TweenManager();
    context->m_rwindow = rw;
    map[Debug] = thor::Action(sf::Keyboard::B, thor::Action::PressOnce);
}

Level::~Level()
{
    //dtor
}

void Level::Prepare()
{
    leveldata = new JSonLevel("nivel_1_1");
    context->LoadWorld(leveldata->getRubeSource(),
                       leveldata->getTileSize(),
                       leveldata->getDimWidth(),
                       leveldata->getDimHeight());

    context->m_script = new LuaScript("assets/Player.lua");
    context->m_psystem->setTexture(*(context->getPrtTexture("blood")));
    groundS.setTexture(*(context->getPrtTexture("floor")));

    GameCL = new sse::ContactListener(context);
    context->m_world -> SetContactListener(GameCL);

    targetS.setTexture(*(context->getPrtTexture("target")));
    pointerS.setTexture(*(context->getPrtTexture("pointer")));
    targetS.setOrigin(targetS.getTextureRect().width / 2.f, targetS.getTextureRect().height / 2.f);
    CurrentTargetS = &targetS;

    CreateGUI();
    CreateHPBar();

    isReady = true;
}

void Level::Restart()
{
    CharacterList.clear();
    DrawList.clear();
    while(!EnemmyList.empty())
    {
        sse::AICharacter* enemmy = EnemmyList.back();
        EnemmyList.pop_back();
        delete enemmy;
    }
    if(player != 0)
        delete player;

    for(auto chd = leveldata->CharacterList.cbegin() ; chd != leveldata->CharacterList.cend() ; chd++ )
    {
        dataCharacter* datacharacter = *chd;
        EnemmyList.push_back(new sse::AICharacter(datacharacter->x,
                                                  datacharacter->y,
                                                  datacharacter->code,context));
    }

    for(auto e = EnemmyList.cbegin() ; e != EnemmyList.cend() ; e++ )
    {
        sse::AICharacter* enemmy = *e;
        CharacterList.push_back(enemmy);
    }
    player = new sse::Player(leveldata->getOriginCharacter().x,
                             leveldata->getOriginCharacter().y,
                             leveldata->getCodePlayer(),
                             context);
    CharacterList.push_back(player);

    for(auto c = CharacterList.cbegin() ; c != CharacterList.cend() ; c++ )
    {
        sse::Character* character = *c;
        character->setBulletList(&BulletList);
        DrawList.push_back(character);
    }

    playerHP = player->getHP();
}

void Level::CreateHPBar()
{
    hpbar.setTexture(*(context->getPrtTexture("hpbar")));
    hpbar.setPosition(10.f, 10.f);
    roundedRecthp = thor::Shapes::roundedRect(sf::Vector2f(200.f, 30.f), 15.f, sf::Color(200, 0, 0), 0.f, sf::Color(0, 0, 0));
    roundedRect = thor::Shapes::roundedRect(sf::Vector2f(200.f, 30.f), 15.f, sf::Color(60, 0, 0), 0.f, sf::Color(0, 0, 0));
    roundedRect.setPosition(80.f, 40.f);
    roundedRecthp.setPosition(80.f, 40.f);
}

void Level::CreateGUI()
{
    /* GUI */
    auto star_button = sfg::Button::Create( "  Menu  " );
    star_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level1::GotoMenu, this ) );
    auto exit_button = sfg::Button::Create( "Exit" );
    exit_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level1::ExitClick, this ) );

    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    //box->Pack( m_label );
    box->Pack( star_button, false );
    box->Pack( exit_button, false );

    window->Add( box );
    // Create a window and add the box layouter to it. Also set the window's title.
    window->SetStyle( window->GetStyle() ^ sfg::Window::TITLEBAR );
    //window->SetStyle( window->GetStyle() ^ sfg::Window::BACKGROUND);
    window->SetStyle( window->GetStyle() ^ sfg::Window::RESIZE );

    desktop.LoadThemeFromFile( "assets/main.theme" );
    desktop.Add( window );

    window->Show(false);
    sf::FloatRect allocation = window->GetAllocation();
    window->SetPosition( sf::Vector2f( context->m_screensize.x - allocation.width, context->m_screensize.y-allocation.height )/2.0f );

    sf::Image sfgui_logo;
	auto image = sfg::Image::Create();
	if( sfgui_logo.loadFromFile( "assets/images/others/icon_dead.png" ) ) {
		image->SetImage( sfgui_logo );
	}
    auto box_dead = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    auto continuar_button = sfg::Button::Create( "  Continuar?  " );
    continuar_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level1::GotoMenu, this ) );
    box_dead->Pack(image,false);
    box_dead->Pack(continuar_button,false);
    dead_window->Add( box_dead );
    dead_window->SetStyle( dead_window->GetStyle() ^ sfg::Window::TITLEBAR );
    dead_window->SetStyle( dead_window->GetStyle() ^ sfg::Window::RESIZE );
    desktop.Add( dead_window );
    dead_window->Show(false);
    /* Fin GUI */
    sf::FloatRect dallocation = dead_window->GetAllocation();
    dead_window->SetPosition( sf::Vector2f( context->m_screensize.x - dallocation.width, context->m_screensize.y-dallocation.height - 200.0f )/2.0f );
}

void Level::CastEnemy()
{
    b2Vec2 p1 = player->Body->GetPosition();
    for(auto e = EnemmyList.cbegin() ; e != EnemmyList.cend() ; e++ )
    {
        sse::AICharacter* enemmy = *e;
        b2Vec2 p3 = enemmy->Body->GetPosition();
        b2Vec2 dif2 = p1 - p3;
        float module2 = sqrt(pow(dif2.x,2)+pow(dif2.y,2));
        b2Vec2 p4 = p3 + b2Vec2(dif2.x/module2*6,dif2.y/module2*6);
        sse::MyRayCastCallback RayCastCallback2;
        context->m_world->RayCast(&RayCastCallback2, p3 , p4);
        if ( RayCastCallback2.m_fixture )
        {
            if(enemmy->Target == 0 && !enemmy->gotoflag)
            {
                sse::UserData* userdataA = static_cast<sse::UserData*>(RayCastCallback2.m_fixture->GetUserData());
                if(userdataA->tipo == 1)
                {
                    enemmy->setAnimCicle(2);
                    enemmy->setTarget(player->Body);
                }
            }
            p4 = b2Vec2(RayCastCallback2.m_point.x, RayCastCallback2.m_point.y);
        }
        if(debugflag)
        {
            sf::Vertex line2[] =
            {
                sf::Vertex(sf::Vector2f(p3.x*PPM, p3.y*PPM)),
                sf::Vertex(sf::Vector2f(p4.x*PPM, p4.y*PPM))
            };
            context->m_rwindow->draw(line2, 2, sf::Lines);
        }
    }
}

void Level::ExitClick()
{
    window->Show(false);
    gotoWin = -1;
}

void Level::GotoMenu()
{
    dead_window->Show(false);
    window->Show(false);
    gotoWin = 0;
}

}
