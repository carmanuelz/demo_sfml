#include "Level1.h"

Level1::Level1()
{
    if(!groundT.loadFromFile("maps/area1.png"))
	{
		return EXIT_FAILURE;
	}
	groundS.setTexture(groundT);

    renderWindow.clear();
    renderWindow.draw(groundS);
    renderWindow.display();

    screnSize = sf::Vector2i((int)renderWindow.getSize().x,(int)renderWindow.getSize().y);

	if (!texture.loadFromFile("assets/bloodparticle.png"))
		return EXIT_FAILURE;

	// Instantiate particle system and add custom affector


    context->m_psystem->setTexture(texture);
    context->m_psystem->addAffector(BloodkAffector());
    context->m_rwindow = &renderWindow;
    context->LoadWorld("maps/nivel1.json");
    context->Createfinder(32,30,30);
    context->m_script = new LuaScript("Player.lua");

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
}

Level1::~Level1()
{
    //dtor
}

int Level1::Run(sf::RenderWindow &App)
