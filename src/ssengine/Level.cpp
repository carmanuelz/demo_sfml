#include "Level.h"

bool Level::compareByY(const sse::drawableentity* a, const sse::drawableentity* b)
{
    return a->y < b->y;
}

Level::Level(sf::RenderWindow* rw, thor::MultiResourceCache* incache, std::string inmane_level):name_level(inmane_level)
{
    rw->clear();
    context = new sse::GameContext(rw);
    context->m_psystem->addAffector(BloodkAffector());
    context->cache = incache;
    context->m_tweenmanager = new sse::TweenManager();
    map[Debug] = thor::Action(sf::Keyboard::B, thor::Action::PressOnce);
}

Level::~Level()
{
    //dtor
}

void Level::Prepare()
{
    leveldata = new JSonLevel(name_level);
    context->LoadWorld(leveldata->getRubeSource(),
                       leveldata->getTileSize(),
                       leveldata->getDimWidth(),
                       leveldata->getDimHeight());

    context->m_script = new LuaScript("assets/Player.lua");
    context->m_psystem->setTexture(*(context->getPrtTexture("blood")));
    groundS.setTexture(*(context->getPrtTexture("floor")));
    topS.setTexture(*(context->getPrtTexture("top")));

    GameCL = new sse::ContactListener(context);
    context->m_world -> SetContactListener(GameCL);

    targetS.setTexture(*(context->getPrtTexture("target")));
    pointerS.setTexture(*(context->getPrtTexture("pointer")));
    hpbar.setTexture(*(context->getPrtTexture("hpbar")));
    hpbar.setPosition(10.f, 10.f);

    targetS.setOrigin(targetS.getTextureRect().width / 2.f, targetS.getTextureRect().height / 2.f);
    CurrentTargetS = &targetS;

    CreateGUI();
    CreateHPBar();
    isReady = true;
}

void Level::Restart()
{
    Clear();
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
    playerposition = player->updateEventListener();
    playerHP = player->getHP();
    isFinish = false;
}

void Level::Clear()
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
    star_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level::GotoMenu, this ) );
    auto exit_button = sfg::Button::Create( "Exit" );
    exit_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level::ExitClick, this ) );

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

    sfgui_dead.loadFromFile( "assets/images/others/icon_dead.png" );
    sfgui_coup.loadFromFile( "assets/images/others/coup.png" );

    auto box_dead = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    auto continuar_button = sfg::Button::Create( "  Continuar?  " );
    continuar_button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level::GotoMenu, this ) );
    box_dead->Pack(image,false);
    box_dead->Pack(continuar_button,false);
    dead_window->Add( box_dead );
    dead_window->SetStyle( dead_window->GetStyle() ^ sfg::Window::TITLEBAR );
    dead_window->SetStyle( dead_window->GetStyle() ^ sfg::Window::RESIZE );
    desktop.Add( dead_window );
    dead_window->Show(false);
    /* Fin GUI */
    sf::FloatRect dallocation = dead_window->GetAllocation();
    dead_window->SetPosition( sf::Vector2f( context->m_screensize.x - dallocation.width, context->m_screensize.y-dallocation.height - 240.0f )/2.0f );
}

void Level::CastEnemy()
{
    for(auto e = EnemmyList.cbegin() ; e != EnemmyList.cend() ; e++ )
    {
        sse::AICharacter* enemmy = *e;
        enemmy->CastTarget(player->Body, debugflag);
    }
}

void Level::EventManagement()
{
    map.clearEvents();
    sf::Event event;
    while(context->m_rwindow->pollEvent(event))
        {
            map.pushEvent(event);

            if (event.type == sf::Event::Closed)
                context->m_rwindow->close();

            if(event.type == sf::Event::MouseEntered)
                isFocused = true;

            if(event.type == sf::Event::MouseLeft)
                isFocused = false;

            if(isFocused)
            {
                if(event.type == sf::Event::MouseMoved)
                {
                    std::vector<sf::FloatRect> allocations;
                    if(window->IsGloballyVisible())
                        allocations.push_back(window->GetAllocation());
                    if(dead_window->IsGloballyVisible())
                        allocations.push_back(dead_window->GetAllocation());
                    hasclickplayer = sse::insideGUI(allocations, winmouseposition);
                }
            }
            else
                hasclickplayer = false;

            if(event.type == sf::Event::KeyPressed)
            {
                if(!player->isDead() && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window->Show(!window->IsGloballyVisible());
                }
                if (map.isActive(Debug))
                    debugflag = !debugflag;
            }
            desktop.HandleEvent( event );
        }
}

void Level::DrawObjects(sf::Time delta)
{
    std::sort(DrawList.begin(),DrawList.end(),compareByY);
    for(std::vector<sse::drawableentity*>::iterator d = DrawList.begin() ; d != DrawList.end() ; d++ )
    {
        sse::drawableentity* entity = *d;
        if(entity->needremove)
        {
            DrawList.erase(d);
            d--;
        }
        else
            entity->draw();
    }
    for(std::vector<sse::Bullet*>::iterator k = BulletList.begin() ; k != BulletList.end() ; k++ )
    {
        sse::Bullet* b = *k;
        if(b->estado == 1)
        {
            BulletList.erase( k );
            delete(b);
            k--;
        }

        else
        {
            b->draw(delta);
        }
    }
}

sf::View Level::ViewManagement()
{
    sf::View view = context->m_rwindow->getView();
    sf::Vector2f offsetview = sf::Vector2f((playerposition.x - view.getCenter().x)*0.1f,(playerposition.y - view.getCenter().y)*0.1f)+ context->impactview;
    if(offsetview.x<0.5 && offsetview.x > -0.5)
        offsetview.x = 0;
    if(offsetview.y<0.5 && offsetview.y > -0.5)
        offsetview.y = 0;

    view.move(offsetview.x,offsetview.y);
    roundedRect.move(offsetview.x, offsetview.y);
    roundedRecthp.move(offsetview.x, offsetview.y);
    hpbar.move(offsetview.x, offsetview.y);

    return view;
}

void Level::UpdateLists(sf::Time delta)
{
    for(std::vector<sse::AICharacter*>::iterator e = EnemmyList.begin() ; e != EnemmyList.end() ; e++ )
    {
        sse::AICharacter* enemmy = *e;
        if(enemmy->needremove)
        {
            EnemmyList.erase(e);
            delete enemmy;
            e--;
        }
        else
            enemmy->update(delta);
    }

    player->update(delta);
}

void Level::DrawPointer(sf::Vector2f mousePos)
{
    if(!hasclickplayer)
        CurrentTargetS = &pointerS;
    else
        CurrentTargetS = &targetS;

    CurrentTargetS->setPosition(mousePos);
    context->m_rwindow->draw(*CurrentTargetS);
}

int Level::Run()
{
    if(!isReady)
        Prepare();
    Restart();

    while(context->m_rwindow->isOpen())
    {
        if(gotoWin >-2)
        {
            int rvalue = gotoWin;
            gotoWin = -2;
            return rvalue;
        }

        winmouseposition = sf::Mouse::getPosition(*context->m_rwindow);

        EventManagement();

        sf::View view = ViewManagement();
        sf::Vector2f mousePos(winmouseposition.x + view.getCenter().x - context->m_screensize.x/2 ,winmouseposition.y + view.getCenter().y - context->m_screensize.y/2);

        if(!player->isDead() && isFocused)
            player->updatebehaviour(mousePos.x,mousePos.y);

        playerposition = player->updateEventListener(isFocused, hasclickplayer);

        sf::Time frameTime = frameClock.restart();

        UpdateLists(frameTime);

        context->m_world->Step( 0.16f, 8, 3 );

        context->m_rwindow->clear(sf::Color(44, 48, 65));

        context->m_rwindow->setView(view);
        context->m_tweenmanager->update(frameTime);

        context->m_rwindow->draw(groundS);
        DrawObjects(frameTime);
        context->m_rwindow->draw(topS);

        if((player->isDead() || EnemmyList.size() == 0) && !isFinish)
        {
            if(EnemmyList.size() == 0)
                image->SetImage( sfgui_coup );
            else
                image->SetImage( sfgui_dead );

            dead_window->Show(true);
            for(auto e = EnemmyList.cbegin() ; e != EnemmyList.cend() ; e++ )
            {
                sse::AICharacter* enemmy = *e;
                enemmy->GoToOrigin();
            }
            isFinish = true;
        }
        else
        {
            if(!player->isDead())
            {
                roundedRecthp.setScale(player->getHP()/playerHP,1);
                CastEnemy();
            }
        }

        context->m_psystem->update(frameTime);
        context->DrawSysParticle();
        context->m_rwindow->draw(roundedRect);
        context->m_rwindow->draw(roundedRecthp);
        context->m_rwindow->draw(hpbar);
        desktop.Update( frameTime.asSeconds() );
        m_sfgui.Display(*(context->m_rwindow));

        if(debugflag)
        {
            context->m_world->DrawDebugData();
            std::stringstream stream;
            const float time = 1.f / frameTime.asSeconds();
            stream << "April's Quest. Current fps: " << time << std::endl;
            context->m_rwindow->setTitle(stream.str());
            context->m_finder->drawdebug(context->m_rwindow);
        }
        DrawPointer(mousePos);
        context->m_rwindow->display();
    }
    return (-1);
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
