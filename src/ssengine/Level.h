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
#include "debugrender.h"
#include "ssengine.h"
#include "Screen.h"
#include "GameContext.h"
#include "Player.h"
#include "Bullet.h"
#include "AICharacter.h"
#include "tween/TweenManager.h"
#include "ContactListener.h"
#include "JSonLevel.h"

enum MyAction
{
	Debug
};

class Level : public Screen
{
public:
    Level(sf::RenderWindow* rw, thor::MultiResourceCache* incache,std::string inmane_level);
    virtual ~Level();
    virtual int Run();
protected:
private:
    sse::GameContext* context;
    JSonLevel* leveldata;
    std::string name_level;

    sse::ContactListener* GameCL;
    sf::Sprite groundS;
    sf::Sprite topS;
    sf::Sprite targetS;
    sf::Sprite pointerS;
    sf::Sprite hpbar;
    sf::Vector2i winmouseposition;
    sf::Vector2f playerposition;

    sf::Sprite* CurrentTargetS;
    float playerHP;
    sse::Player* player = 0;
    sf::ConvexShape roundedRecthp;
    sf::ConvexShape roundedRect;
    sf::Clock frameClock;

    sfg::SFGUI m_sfgui;
    sfg::Label::Ptr m_label;
    sfg::Window::Ptr window = sfg::Window::Create();
    sfg::Desktop desktop;

    sf::Image sfgui_dead;
    sf::Image sfgui_coup;
    sfg::Image::Ptr image = sfg::Image::Create();
    sfg::Window::Ptr dead_window = sfg::Window::Create();

    std::vector<sse::drawableentity*> DrawList;
    std::vector<sse::AICharacter*> EnemmyList;
    std::vector<sse::Character*> CharacterList;
    std::vector<sse::Bullet*> BulletList;

    bool isFocused = true;
    bool hasclickplayer = true;
    bool debugflag = false;
    bool isReady = false;
    bool isFinish = false;
    void ExitClick();
    void GotoMenu();
    void Prepare();
    void Restart();
    void CreateGUI();
    void CreateHPBar();
    void CastEnemy();
    void Clear();
    void EventManagement();
    void DrawObjects(sf::Time delta);
    void DrawPointer(sf::Vector2f mousePos);
    sf::View ViewManagement();
    void UpdateLists(sf::Time delta);
    int gotoWin = -2;
    thor::ActionMap<MyAction> map;
    static bool compareByY(const sse::drawableentity* a,const sse::drawableentity* b);
};

#endif // LEVEL_H
