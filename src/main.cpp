#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFGUI/SFGUI.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Shapes/ConcaveShape.hpp>
#include <Thor/Shapes/Shapes.hpp>

#include "rubestuff/b2dJson.h"
#include "debugrender.h"
#include "ssengine/ssengine.h"
#include "GameContext.h"
#include <math.h>
#include <sstream>

sse::GameContext* context;
std::vector<sse::MyRayCastCallback*> RaycastList;

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        b2Fixture* FixtA = contact->GetFixtureA();
        b2Fixture* FixtB = contact->GetFixtureB();
        sse::UserData* userdataA = static_cast<sse::UserData*>(FixtA->GetUserData());
        sse::UserData* userdataB = static_cast<sse::UserData*>(FixtB->GetUserData());
        if(userdataA->tipo == 3 /*&& userdataB->tipo == 2*/ &&userdataA->estado==0)
        {
            context->RemoveList.push_back(FixtA->GetBody());
            userdataA->estado=1;
        }
        else
            if(userdataB->tipo == 3 /*&& userdataA->tipo == 2*/&& userdataB->estado==0)
            {
                context->RemoveList.push_back(FixtB->GetBody());
                userdataB->estado=1;
            }

        if(userdataA->tipo == 4)
        {
            b2Body* bodyA = FixtA->GetBody();
            sse::Character* collCharacter = static_cast<sse::Character*>(bodyA->GetUserData());
            collCharacter->addCollisionList(FixtB);
        }
        else
            if(userdataB->tipo == 4)
            {
                b2Body* bodyB = FixtB->GetBody();
                sse::Character* collCharacter = static_cast<sse::Character*>(bodyB->GetUserData());
                collCharacter->addCollisionList(FixtA);
            }
    }

    void EndContact(b2Contact* contact)
    {
        b2Fixture* FixtA = contact->GetFixtureA();
        b2Fixture* FixtB = contact->GetFixtureB();
        sse::UserData* userdataA = static_cast<sse::UserData*>(FixtA->GetUserData());
        sse::UserData* userdataB = static_cast<sse::UserData*>(FixtB->GetUserData());

        if(userdataA->tipo == 4)
        {
            b2Body* bodyA = FixtA->GetBody();
            sse::Character* collCharacter = static_cast<sse::Character*>(bodyA->GetUserData());
            collCharacter->removeCollisionList(FixtB);
        }
        else
            if(userdataB->tipo == 4)
            {
                b2Body* bodyB = FixtB->GetBody();
                sse::Character* collCharacter = static_cast<sse::Character*>(bodyB->GetUserData());
                collCharacter->removeCollisionList(FixtA);
            }
    }
};

int main()
{
    std::string errMsg;
    sf::Clock stepClock;
    sf::VideoMode videomode(800, 600, 32);
	sf::RenderWindow renderWindow(videomode, "Test"/*,sf::Style::Fullscreen*/);
	renderWindow.setVerticalSyncEnabled(true);
    DebugDraw debugDraw = DebugDraw(renderWindow);
    renderWindow.setFramerateLimit(60);
    //renderWindow.setMouseCursorVisible(false);
    sf::Vector2i screnSize((int)renderWindow.getSize().x,(int)renderWindow.getSize().y);

    sf::Texture texture;
	if (!texture.loadFromFile("assets/bloodparticle.png"))
		return EXIT_FAILURE;

	// Instantiate particle system and add custom affector

	context = new sse::GameContext();
    context->m_psystem->setTexture(texture);
    context->m_psystem->addAffector(BloodkAffector());
    context->m_rwindow = &renderWindow;
    context->LoadWorld("maps/nivel1.json");
    context->Createfinder(32,30,30);
    context->m_script = new LuaScript("Player.lua");


    sf::Texture groundT;
    if(!groundT.loadFromFile("maps/area1.png"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite groundS;
	groundS.setTexture(groundT);
	b2dJson json;

    ContactListener GameCL;
	context->m_world -> SetContactListener(&GameCL);

    sse::AICharacter* character = new sse::AICharacter(700,400,"mob001",context);
    sse::Player* player = new sse::Player(100,100,"player01",context);
    sf::Vector2f* impactview = &(player->moveimpactview);
    float playerHP = player->getHP();

    sf::Sprite targetS;
    sf::Texture targetT;

	if(!targetT.loadFromFile("assets/target.png"))
	{
		return EXIT_FAILURE;
	}

	targetS.setTexture(targetT);
	sf::Vector2f centro;
	centro.x = targetS.getTextureRect().width / 2.f;
	centro.y = targetS.getTextureRect().height / 2.f;
	targetS.setOrigin(centro);

    //character->setTarget(player->Body);

    debugDraw.SetFlags( b2Draw::e_shapeBit );
    context->m_world->SetDebugDraw(&debugDraw);

	sf::Clock frameClock;
	//-----------------------------------//

	sf::ConvexShape roundedRecthp = thor::Shapes::roundedRect(sf::Vector2f(200.f, 15.f), 3.f, sf::Color(200, 0, 0), 0.f, sf::Color(0, 0, 0));
	sf::ConvexShape roundedRect = thor::Shapes::roundedRect(sf::Vector2f(200.f, 15.f), 3.f, sf::Color(60, 0, 0), 3.f, sf::Color(180, 200, 200));

	roundedRect.setPosition(50.f, 50.f);
	roundedRecthp.setPosition(50.f, 50.f);

	AnimatedAccessor* animaccess = new AnimatedAccessor();
    sse::Tween tween;
    float* values= new float[3];
    values[0] = 0;
    values[1] = 255;
    values[2] = 0;

    sse::tweenOptions* options = new sse::tweenOptions();
    options->target = &(player->animated);
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
    // Create an SFGUI. This is required before doing anything with SFGUI.
    sfg::SFGUI m_sfgui;

    // Create the label pointer here to reach it from OnButtonClick().
    sfg::Label::Ptr m_label;

    // Create the label.
	m_label = sfg::Label::Create( "Hello world!" );

	// Create a simple button and connect the click signal.
	auto button = sfg::Button::Create( "Greet SFGUI!" );

    auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack( m_label );
	box->Pack( button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	auto window = sfg::Window::Create();
	window->SetTitle( "Hello world!" );
	window->SetPosition( sf::Vector2f( 100.f, 100.f ) );
	window->Add( box );

	sfg::Desktop desktop;
	desktop.Add( window );

	// We're not using SFML to render anything in this program, so reset OpenGL
	// states. Otherwise we wouldn't see anything.
	//render_window.resetGLStates();
	/**/

	bool isFocused = true;
    bool hasclickplayer = true;
    sf::Vector2f VURef(0,0);

    //poll input
    sf::Event event;
	while(renderWindow.isOpen())
	{
	    sf::Vector2i winmouseposition = sf::Mouse::getPosition(renderWindow);

		while(renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();

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
                    return EXIT_SUCCESS;
                }


		    desktop.HandleEvent( event );
        }
        sf::View view = renderWindow.getView();
        sf::Vector2f mousePos(winmouseposition.x + view.getCenter().x - screnSize.x/2 ,winmouseposition.y + view.getCenter().y - screnSize.y/2);
        sf::Vector2f playerposition = player->updatePlayer(isFocused, hasclickplayer);
        if(isFocused)
            VURef = player->updatebehaviour(mousePos.x,mousePos.y);
        sf::Time frameTime = frameClock.restart();
        character->updateFind();
        character->update(frameTime);
        player->update(frameTime);

        context->m_world->Step( 0.16f, 8, 3 );

		renderWindow.clear();
		renderWindow.draw(groundS);

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
        renderWindow.setView(view);

        player->draw();
        character->draw();

        targetS.setPosition(mousePos);
        renderWindow.draw(targetS);

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
            renderWindow.draw(*bulletS);
        }

        /*raycast*/
        b2Vec2 p1 = player->Body->GetPosition();

        /*int numRays = 20;
        for (int i = 0; i < numRays; i++) {
            float angle = (i / (float)numRays) * 360 * DEGTORAD;
            b2Vec2 rayDir( sinf(angle), cosf(angle) );
            b2Vec2 p2 = p1 + 5 * rayDir;

            //check what this ray hits
            sse::MyRayCastCallback callback;//basic callback to record body and hit point
            m_world->RayCast(&callback, p1, p2);
            if ( callback.m_fixture )
                p2 = b2Vec2(callback.m_point.x, callback.m_point.y);

            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(p1.x*PPM, p1.y*PPM)),
                sf::Vertex(sf::Vector2f(p2.x*PPM, p2.y*PPM))
            };

            renderWindow.draw(line, 2, sf::Lines);
        }*/
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
		//renderWindow.draw(context->obj_psystem);
		renderWindow.draw(roundedRect);
		renderWindow.draw(roundedRecthp);

		desktop.Update( frameTime.asSeconds() );
		m_sfgui.Display( renderWindow );

        renderWindow.draw(line2, 2, sf::Lines);
        //m_world->DrawDebugData();
		renderWindow.display();
		const float time = 1.f / frameClock.getElapsedTime().asSeconds();
		std::stringstream stream;
		stream << "Use the cursor keys to move the view. Current fps: " << time << std::endl;
		renderWindow.setTitle(stream.str());

	}

	return 0;
}
