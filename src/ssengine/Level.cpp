
#include "Level.h"

namespace sse
{
bool Level::compareByY(const sse::drawableentity* a, const sse::drawableentity* b)
{
    return a->y < b->y;
}

Level::Level(sf::RenderWindow* rw, thor::MultiResourceCache* incache)
{
    context = new sse::GameContext(rw);
    context->m_psystem->addAffector(BloodkAffector());

    context->m_tweenmanager = new sse::TweenManager();
    context->m_rwindow = rw;
    context->LoadWorld("maps/nivel1.json",32,30,30);
}

Level::~Level()
{
    //dtor
}

int Level::Run()
{

}

void Level::CastEnemy()
{
}

}
