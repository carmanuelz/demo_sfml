
#include "Level.h"

namespace sse
{
bool Level::compareByY(const sse::drawableentity* a, const sse::drawableentity* b)
{
    return a->y < b->y;
}

Level::Level(sf::RenderWindow* rw)
{

    context = new sse::GameContext();
    context->m_psystem->setTexture(texture);
    context->m_psystem->addAffector(BloodkAffector());

    context->m_tweenmanager = new sse::TweenManager();
    context->m_rwindow = rw;
    context->LoadWorld("maps/nivel1.json");
    context->Createfinder(32,30,30);
    context->m_script = new LuaScript("Player.lua");

    groundT.loadFromFile("maps/area1.png");
    groundS.setTexture(groundT);

    context->m_rwindow->clear();
    context->m_rwindow->draw(groundS);
    context->m_rwindow->display();

    screnSize = sf::Vector2i((int)context->m_rwindow->getSize().x,(int)context->m_rwindow->getSize().y);

    texture.loadFromFile("assets/bloodparticle.png");

    map[Debug] = thor::Action(sf::Keyboard::B, thor::Action::PressOnce);

    // Instantiate particle system and add custom affector

    debugDraw = new DebugDraw(*(context->m_rwindow));

    debugDraw->SetFlags(b2Draw::e_shapeBit);
    context->m_world->SetDebugDraw(debugDraw);

    GameCL = new ContactListener(context);
    context->m_world -> SetContactListener(GameCL);

    EnemyList.push_back(new sse::AICharacter(700,400,"mob001",context));
    EnemyList.push_back(new sse::AICharacter(700,500,"mob001",context));
    EnemyList.push_back(new sse::AICharacter(700,600,"mob001",context));

    for(auto e = EnemyList.cbegin() ; e != EnemyList.cend() ; e++ )
    {
        sse::AICharacter* enemmy = *e;
        CharacterList.push_back(enemmy);
    }
    player = new sse::Player(100,100,"player01",context);
    CharacterList.push_back(player);

    for(auto c = CharacterList.cbegin() ; c != CharacterList.cend() ; c++ )
    {
        sse::Character* character = *c;
        DrawList.push_back(character);
    }

    impactview = &(player->moveimpactview);
    playerHP = player->getHP();

    targetT.loadFromFile("assets/target.png");
    pointerT.loadFromFile("assets/pointer.png");

    targetS.setTexture(targetT);
    pointerS.setTexture(pointerT);

    centro.x = targetS.getTextureRect().width / 2.f;
    centro.y = targetS.getTextureRect().height / 2.f;
    targetS.setOrigin(centro);
    CurrentTargetS = &targetS;
    //-----------------------------------//

    roundedRecthp = thor::Shapes::roundedRect(sf::Vector2f(200.f, 15.f), 3.f, sf::Color(200, 0, 0), 0.f, sf::Color(0, 0, 0));
    roundedRect = thor::Shapes::roundedRect(sf::Vector2f(200.f, 15.f), 3.f, sf::Color(60, 0, 0), 3.f, sf::Color(180, 200, 200));

    roundedRect.setPosition(50.f, 50.f);
    roundedRecthp.setPosition(50.f, 50.f);

    m_label = sfg::Label::Create( "Hello world!" );

    // Create a simple button and connect the click signal.
    auto button = sfg::Button::Create( "Greet SFGUI!" );
    button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level::OnButtonClick, this ) );

    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
    box->Pack( m_label );
    box->Pack( button, false );

    // Create a window and add the box layouter to it. Also set the window's title.
    window->SetTitle( "Hello world!" );
    window->SetPosition( sf::Vector2f( 100.f, 100.f ) );
    window->Add( box );

    desktop.Add( window );
}

Level::~Level()
{
    //dtor
}


void Level::OnButtonClick()
{
    m_label->SetText( "Hello SFGUI, pleased to meet you!" );
}

int Level::Run()
{
    while(context->m_rwindow->isOpen())
    {
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
                    allocations.push_back(window->GetAllocation());
                    hasclickplayer = sse::insideGUI(allocations, winmouseposition);
                }
            }
            else
                hasclickplayer = false;

            if(event.type == sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    return -1;
                }
                if (map.isActive(Debug))
                    debugflag = !debugflag;
            }


            desktop.HandleEvent( event );
        }
        sf::View view = context->m_rwindow->getView();
        sf::Vector2f mousePos(winmouseposition.x + view.getCenter().x - screnSize.x/2 ,winmouseposition.y + view.getCenter().y - screnSize.y/2);
        sf::Vector2f playerposition = player->updatePlayer(isFocused, hasclickplayer);

        if(!player->isDead() && isFocused)
            player->updatebehaviour(mousePos.x,mousePos.y);

        sf::Time frameTime = frameClock.restart();

        for(std::vector<sse::AICharacter*>::iterator e = EnemyList.begin() ; e != EnemyList.end() ; e++ )
        {
            sse::AICharacter* enemmy = *e;
            if(enemmy->needremove)
            {
                EnemyList.erase(e);
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

        context->m_world->Step( 0.16f, 8, 3 );

        context->m_rwindow->clear();
        context->m_rwindow->draw(groundS);

        stepClock.restart();
        sf::Vector2f offsetview = sf::Vector2f((playerposition.x - view.getCenter().x)*0.1f,(playerposition.y - view.getCenter().y)*0.1f)+ *impactview;
        if(offsetview.x<0.5 && offsetview.x > -0.5)
            offsetview.x = 0;
        if(offsetview.y<0.5 && offsetview.y > -0.5)
            offsetview.y = 0;

        view.move(offsetview.x,offsetview.y);
        roundedRect.move(offsetview.x, offsetview.y);
        roundedRecthp.move(offsetview.x, offsetview.y);
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
               BulletList.erase( k );
               k--;
            }
        }

        b2Vec2 p1 = player->Body->GetPosition();

        context->m_psystem->update(frameTime);
        context->DrawSysParticle();

        context->m_rwindow->draw(roundedRect);
        context->m_rwindow->draw(roundedRecthp);

        desktop.Update( frameTime.asSeconds() );
        m_sfgui.Display(*(context->m_rwindow));
        if(debugflag)
            context->m_world->DrawDebugData();
        context->m_rwindow->draw(*CurrentTargetS);
        context->m_rwindow->display();
        const float time = 1.f / frameClock.getElapsedTime().asSeconds();
        std::stringstream stream;
        stream << "Use the cursor keys to move the view. Current fps: " << time << std::endl;
        context->m_rwindow->setTitle(stream.str());
    }
    return (-1);
}

void Level::CastEnemy()
{
    b2Vec2 p1 = player->Body->GetPosition();

    for(auto e = EnemyList.cbegin() ; e != EnemyList.cend() ; e++ )
    {
        sse::AICharacter* enemy = *e;
        b2Vec2 p3 = enemy->Body->GetPosition();
        b2Vec2 dif2 = p1 - p3;
        float module2 = sqrt(pow(dif2.x,2)+pow(dif2.y,2));
        b2Vec2 p4 = p3 + b2Vec2(dif2.x/module2*4,dif2.y/module2*4);
        sse::MyRayCastCallback RayCastCallback2;
        context->m_world->RayCast(&RayCastCallback2, p3 , p4);
        if ( RayCastCallback2.m_fixture )
        {
            if(enemy->Target == 0 && !enemy->gotoflag)
            {
                sse::UserData* userdataA = static_cast<sse::UserData*>(RayCastCallback2.m_fixture->GetUserData());
                if(userdataA->tipo == 1)
                {
                    enemy->setAnimCicle(2);
                    enemy->setTarget(player->Body);
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

}
