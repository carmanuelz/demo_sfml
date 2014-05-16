#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFGUI/SFGUI.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Shapes/ConcaveShape.hpp>
#include <Thor/Shapes/Shapes.hpp>
#include <Thor/Input/Action.hpp>
#include <Thor/Input/ActionMap.hpp>
#include "../debugrender.h"
#include "ssengine.h"
#include "Screen.h"
#include "GameContext.h"
#include "Player.h"
#include "AICharacter.h"
#include "tween/TweenManager.h"
#include "ContactListener.h"

namespace sse
{
enum Actions
{
	Debug
};

class Level : public Screen
{
public:
    Level(sf::RenderWindow* rw);
    virtual ~Level();
    virtual int Run();
protected:
    void CastEnemy();
private:
    DebugDraw* debugDraw;
    sse::GameContext* context;
    ContactListener* GameCL;
    std::vector<sse::MyRayCastCallback*> RaycastList;
    sf::Texture groundT;
    sf::Sprite groundS;
    sf::Vector2i screnSize;
    sf::Texture texture;
    sf::Vector2f VURef;
    sf::Vector2f centro;
    sf::Sprite targetS;
    sf::Texture targetT;
    sf::Sprite pointerS;
    sf::Texture pointerT;
    sf::Sprite* CurrentTargetS;
    float playerHP;
    sse::Player* player;
    sf::Vector2f* impactview;
    sf::Clock frameClock;
    sf::ConvexShape roundedRecthp;
    sf::ConvexShape roundedRect;
    sf::Clock stepClock;

    sfg::SFGUI m_sfgui;
    sfg::Label::Ptr m_label;
    sfg::Window::Ptr window = sfg::Window::Create();
    sfg::Desktop desktop;

    std::vector<sse::drawableentity*> DrawList;
    std::vector<sse::AICharacter*> EnemyList;
    std::vector<sse::Character*> CharacterList;

    bool isFocused = true;
    bool hasclickplayer = true;
    bool debugflag = false;
    void OnButtonClick();
    static bool compareByY(const sse::drawableentity* a,const sse::drawableentity* b);
    thor::ActionMap<Actions> map;
};
}
#endif // LEVEL_H

