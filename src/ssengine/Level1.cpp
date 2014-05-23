#include "Level1.h"

bool Level1::compareByY(const sse::drawableentity* a, const sse::drawableentity* b)
{
    return a->y < b->y;
}

Level1::Level1(sf::RenderWindow* rw, thor::MultiResourceCache* incache)
{
    context = new sse::GameContext(rw);
    context->m_psystem->addAffector(BloodkAffector());
    context->cache = incache;
    context->m_tweenmanager = new sse::TweenManager();
    map[Debug] = thor::Action(sf::Keyboard::B, thor::Action::PressOnce);

    roundedRecthp = thor::Shapes::roundedRect(sf::Vector2f(200.f, 30.f), 15.f, sf::Color(200, 0, 0), 0.f, sf::Color(0, 0, 0));
    roundedRect = thor::Shapes::roundedRect(sf::Vector2f(200.f, 30.f), 15.f, sf::Color(60, 0, 0), 0.f, sf::Color(0, 0, 0));

    roundedRect.setPosition(80.f, 40.f);
    roundedRecthp.setPosition(80.f, 40.f);
}

Level1::~Level1()
{
    //dtor
}

void Level1::Prepare()
{
    context->LoadWorld("maps/nivel1.json",32,30,30);
    context->m_script = new LuaScript("Player.lua");
    context->m_psystem->setTexture(*(context->getPrtTexture("blood")));
    groundS.setTexture(*(context->getPrtTexture("floor")));

    GameCL = new sse::ContactListener(context);
    context->m_world -> SetContactListener(GameCL);

    targetS.setTexture(*(context->getPrtTexture("target")));
    pointerS.setTexture(*(context->getPrtTexture("pointer")));
    hpbar.setTexture(*(context->getPrtTexture("hpbar")));
    hpbar.setPosition(10.f, 10.f);

    targetS.setOrigin(targetS.getTextureRect().width / 2.f, targetS.getTextureRect().height / 2.f);
    CurrentTargetS = &targetS;

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

    desktop.LoadThemeFromFile( "assets/example.theme" );
    desktop.Add( window );

    window->Show(false);
    sf::FloatRect allocation = window->GetAllocation();
    window->SetPosition( sf::Vector2f( context->m_screensize.x - allocation.width, context->m_screensize.y-allocation.height )/2.0f );

    sf::Image sfgui_logo;
	auto image = sfg::Image::Create();
	if( sfgui_logo.loadFromFile( "assets/icon_dead.png" ) ) {
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
    isReady = true;
}

void Level1::Restart()
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

    EnemmyList.push_back(new sse::AICharacter(700,400,"mob001",context));
    EnemmyList.push_back(new sse::AICharacter(700,500,"mob001",context));
    EnemmyList.push_back(new sse::AICharacter(700,600,"mob001",context));
    EnemmyList.push_back(new sse::AICharacter(650,400,"mob001",context));
    EnemmyList.push_back(new sse::AICharacter(650,350,"mob001",context));
    EnemmyList.push_back(new sse::AICharacter(400,350,"mob001",context));

    for(auto e = EnemmyList.cbegin() ; e != EnemmyList.cend() ; e++ )
    {
        sse::AICharacter* enemmy = *e;
        CharacterList.push_back(enemmy);
    }
    player = new sse::Player(100,100,"player01",context);
    CharacterList.push_back(player);

    for(auto c = CharacterList.cbegin() ; c != CharacterList.cend() ; c++ )
    {
        sse::Character* character = *c;
        character->setBulletList(&BulletList);
        DrawList.push_back(character);
    }

    playerHP = player->getHP();
}

void Level1::ExitClick()
{
    window->Show(false);
    gotoWin = -1;
}

void Level1::GotoMenu()
{
    dead_window->Show(false);
    window->Show(false);
    gotoWin = 0;
}


int Level1::Run()
{
    if(!isReady)
        Prepare();
    Restart();
    bool isFinish = false;
    while(context->m_rwindow->isOpen())
    {
        if(gotoWin >-2)
        {
            int rvalue = gotoWin;
            gotoWin = -2;
            return rvalue;
        }
        map.clearEvents();
        sf::Event event;
        sf::Vector2i winmouseposition = sf::Mouse::getPosition(*context->m_rwindow);
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
        sf::View view = context->m_rwindow->getView();
        sf::Vector2f mousePos(winmouseposition.x + view.getCenter().x - context->m_screensize.x/2 ,winmouseposition.y + view.getCenter().y - context->m_screensize.y/2);
        sf::Vector2f playerposition = player->updatePlayer(isFocused, hasclickplayer);

        sf::Time frameTime = frameClock.restart();

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
                enemmy->updateFind();
        }

        for(std::vector<sse::Character*>::iterator c = CharacterList.begin() ; c != CharacterList.end() ; c++ )
        {
            sse::Character* character = *c;
            if(character->needremove)
            {
                CharacterList.erase(c);
                c--;
            }
            else
                character->update(frameTime);
        }

        if(!player->isDead() && isFocused)
            player->updatebehaviour(mousePos.x,mousePos.y);

        context->m_world->Step( 0.16f, 8, 3 );

        context->m_rwindow->clear();
        context->m_rwindow->draw(groundS);

        stepClock.restart();
        sf::Vector2f offsetview = sf::Vector2f((playerposition.x - view.getCenter().x)*0.1f,(playerposition.y - view.getCenter().y)*0.1f)+ context->impactview;
        if(offsetview.x<0.5 && offsetview.x > -0.5)
            offsetview.x = 0;
        if(offsetview.y<0.5 && offsetview.y > -0.5)
            offsetview.y = 0;

        view.move(offsetview.x,offsetview.y);
        roundedRect.move(offsetview.x, offsetview.y);
        roundedRecthp.move(offsetview.x, offsetview.y);
        hpbar.move(offsetview.x, offsetview.y);
        if(player->getHP() >= 0)
            roundedRecthp.setScale(player->getHP()/playerHP,1);
        context->m_rwindow->setView(view);

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

        if(!hasclickplayer)
            CurrentTargetS = &pointerS;
        else
            CurrentTargetS = &targetS;

        CurrentTargetS->setPosition(mousePos);

        context->m_tweenmanager->update(frameTime);

        while(!RemoveList.empty())
        {
            b2Body* b = RemoveList.back();
            context->m_world -> DestroyBody(b);
            RemoveList.pop_back();
        }

        for(std::vector<sse::Bullet*>::iterator k = BulletList.begin() ; k != BulletList.end() ; k++ )
        {
            sse::Bullet* b = *k;
            if(b->estado == 0)
                b->draw(frameTime);
            else
            {
                delete(b);
                BulletList.erase( k );
                k--;
            }
        }

        b2Vec2 p1 = player->Body->GetPosition();

        if(!player->isDead())
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

        if(player->isDead() && !isFinish)
        {
            dead_window->Show(true);
            for(auto e = EnemmyList.cbegin() ; e != EnemmyList.cend() ; e++ )
            {
                sse::AICharacter* enemmy = *e;
                enemmy->GoToOrigin();
            }
            isFinish = true;
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
        }

        context->m_rwindow->draw(*CurrentTargetS);
        context->m_rwindow->display();
    }
    return (-1);
}
