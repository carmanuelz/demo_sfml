#include "Level1.h"

Level1::Level1(sf::RenderWindow* rw)
{
    renderWindow = rw;

    groundT.loadFromFile("maps/area1.png");
	groundS.setTexture(groundT);

    renderWindow->clear();
    renderWindow->draw(groundS);
    renderWindow->display();

    screnSize = sf::Vector2i((int)renderWindow->getSize().x,(int)renderWindow->getSize().y);

	texture.loadFromFile("assets/bloodparticle.png");

	// Instantiate particle system and add custom affector
    context = new sse::GameContext();
    context->m_psystem->setTexture(texture);
    context->m_psystem->addAffector(BloodkAffector());

    context->m_rwindow = renderWindow;
    context->LoadWorld("maps/nivel1.json");
    context->Createfinder(32,30,30);
    context->m_script = new LuaScript("Player.lua");

    debugDraw = new DebugDraw(*renderWindow);

    debugDraw->SetFlags(b2Draw::e_shapeBit);
    context->m_world->SetDebugDraw(debugDraw);

    GameCL = new ContactListener(context);
	context->m_world -> SetContactListener(GameCL);

    character = new sse::AICharacter(700,400,"mob001",context);
    player = new sse::Player(100,100,"player01",context);

    DrawList.push_back(character);
    DrawList.push_back(player);

    impactview = &(player->moveimpactview);
    playerHP = player->getHP();

	targetT.loadFromFile("assets/target.png");

	targetS.setTexture(targetT);
	centro.x = targetS.getTextureRect().width / 2.f;
	centro.y = targetS.getTextureRect().height / 2.f;
	targetS.setOrigin(centro);
	//-----------------------------------//

	roundedRecthp = thor::Shapes::roundedRect(sf::Vector2f(200.f, 15.f), 3.f, sf::Color(200, 0, 0), 0.f, sf::Color(0, 0, 0));
	roundedRect = thor::Shapes::roundedRect(sf::Vector2f(200.f, 15.f), 3.f, sf::Color(60, 0, 0), 3.f, sf::Color(180, 200, 200));

	roundedRect.setPosition(50.f, 50.f);
	roundedRecthp.setPosition(50.f, 50.f);

	animaccess = new AnimatedAccessor();
    float* values= new float[3];
    values[0] = 255;
    values[1] = 0;
    values[2] = 0;

    options = new sse::tweenOptions();
    options->target = &(character->animated);
    options->accessor = animaccess;
    options->easefunc = sse::easing::easeInLinear;
    options->tweentype = animaccess->COLOR;
    options->destination = values;
    options->sizevalue = 3;
    options->yoyo = true;
    options->repeatCnt = 0;
    options->timeCicle = 0.2f;
    tween.to(options);

	/**/
	m_label = sfg::Label::Create( "Hello world!" );

	// Create a simple button and connect the click signal.
	auto button = sfg::Button::Create( "Greet SFGUI!" );
    button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &Level1::OnButtonClick, this ) );

    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack( m_label );
	box->Pack( button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	window->SetTitle( "Hello world!" );
	window->SetPosition( sf::Vector2f( 100.f, 100.f ) );
	window->Add( box );

	desktop.Add( window );
}

Level1::~Level1()
{
    //dtor
}


void Level1::OnButtonClick() {
	m_label->SetText( "Hello SFGUI, pleased to meet you!" );
}

int Level1::Run()
{
    while(renderWindow->isOpen())
	{
        sf::Vector2i winmouseposition = sf::Mouse::getPosition(*renderWindow);

        while(renderWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                renderWindow->close();

            if(event.type == sf::Event::MouseEntered)
                isFocused = true;

            if(event.type == sf::Event::MouseLeft)
                isFocused = false;

            if(isFocused)
            {
                if(event.type == sf::Event::MouseButtonPressed)
                {
                    std::vector<sf::FloatRect> allocations;
                    allocations.push_back(window->GetAllocation());
                    hasclickplayer = sse::insideGUI(allocations, winmouseposition);
                }
            }
            else
                hasclickplayer = false;

            if(event.type == sf::Event::KeyPressed)
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    return -1;
                }
            desktop.HandleEvent( event );
        }
        sf::View view = renderWindow->getView();
        sf::Vector2f mousePos(winmouseposition.x + view.getCenter().x - screnSize.x/2 ,winmouseposition.y + view.getCenter().y - screnSize.y/2);
        sf::Vector2f playerposition = player->updatePlayer(isFocused, hasclickplayer);
        if(isFocused)
            VURef = player->updatebehaviour(mousePos.x,mousePos.y);
        sf::Time frameTime = frameClock.restart();
        character->updateFind();
        character->update(frameTime);
        player->update(frameTime);

        context->m_world->Step( 0.16f, 8, 3 );

        renderWindow->clear();
        renderWindow->draw(groundS);

        stepClock.restart();
        sf::Vector2f offsetview = sf::Vector2f((playerposition.x - view.getCenter().x)*0.1f,(playerposition.y - view.getCenter().y)*0.1f)+ *impactview;
        if(offsetview.x<0.5 && offsetview.x > -0.5)
            offsetview.x = 0;
        if(offsetview.y<0.5 && offsetview.y > -0.5)
            offsetview.y = 0;

        view.move(offsetview.x,offsetview.y);
        roundedRect.move(offsetview.x, offsetview.y);
        roundedRecthp.move(offsetview.x, offsetview.y);
        if(player->getHP() > 0)
            roundedRecthp.setScale(player->getHP()/playerHP,1);
        renderWindow->setView(view);

        for(auto d = DrawList.cbegin() ; d != DrawList.cend() ; d++ )
        {
            sse::drawableentity* entity = *d;
            entity->draw();
        }

        targetS.setPosition(mousePos);

        tween.update(frameTime);

        while(!context->RemoveList.empty())
        {
            b2Body* b = context->RemoveList.back();
            context->m_world -> DestroyBody(b);
            context->RemoveList.pop_back();
            std::vector<b2Body*>::iterator it = std::find(context->BulletList.begin(), context->BulletList.end(), b);
            if ( it != context->BulletList.end() )
                context->BulletList.erase( it );
        }

        for(auto k = context->BulletList.cbegin() ; k != context->BulletList.cend() ; k++ )
        {
            b2Body* b = *k;
            sf::Sprite* bulletS(static_cast<sf::Sprite*>(b->GetUserData()));
            bulletS->setRotation(b->GetAngle()*RADTODEG);
            bulletS->setPosition( b->GetPosition().x*PPM, b->GetPosition().y*PPM);
            renderWindow->draw(*bulletS);
        }

        b2Vec2 p1 = player->Body->GetPosition();

        b2Vec2 p3 = character->Body->GetPosition();
        b2Vec2 dif2 = p1 - p3;
        float module2 = sqrt(pow(dif2.x,2)+pow(dif2.y,2));
        b2Vec2 p4 = p3 + b2Vec2(dif2.x/module2*4,dif2.y/module2*4);
        sse::MyRayCastCallback RayCastCallback2;
        context->m_world->RayCast(&RayCastCallback2, p3 , p4);
        if ( RayCastCallback2.m_fixture )
        {
            if(character->Target == 0)
            {
                sse::UserData* userdataA = static_cast<sse::UserData*>(RayCastCallback2.m_fixture->GetUserData());
                if(userdataA->tipo == 1)
                {
                    character->setAnimCicle(2);
                    character->setTarget(player->Body);
                }
            }
            p4 = b2Vec2(RayCastCallback2.m_point.x, RayCastCallback2.m_point.y);
        }


        sf::Vertex line2[] =
            {
                sf::Vertex(sf::Vector2f(p3.x*PPM, p3.y*PPM)),
                sf::Vertex(sf::Vector2f(p4.x*PPM, p4.y*PPM))
            };

        context->m_psystem->update(frameTime);
        context->DrawSysParticle();

        renderWindow->draw(roundedRect);
        renderWindow->draw(roundedRecthp);

        desktop.Update( frameTime.asSeconds() );
        m_sfgui.Display( *renderWindow );

        //renderWindow->draw(line2, 2, sf::Lines);*/

        //context->m_world->DrawDebugData();
        renderWindow->draw(targetS);
        renderWindow->display();
        const float time = 1.f / frameClock.getElapsedTime().asSeconds();
        std::stringstream stream;
        stream << "Use the cursor keys to move the view. Current fps: " << time << std::endl;
        renderWindow->setTitle(stream.str());
	}
	return (-1);
}
