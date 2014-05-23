#include "GameContext.h"

namespace sse
{
GameContext::GameContext(sf::RenderWindow* inrw):m_rwindow(inrw)
{
    m_screensize = sf::Vector2i((int)inrw->getSize().x,(int)inrw->getSize().y);
    registerResurces();
}

GameContext::~GameContext()
{
    //dtor
}

bool GameContext::LoadWorld(const char* filename, float tilesize, int width, int height)
{
    m_world = m_b2json->readFromFile(filename, errMsg);
    if ( ! m_world )
    {
        std::cout << "Failed to load scene"<<std::endl;
        return false;
    }
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
    {
        int tipob = m_b2json->getCustomInt(b, "tipo");
        sse::UserData* ud = new sse::UserData();
        ud->tipo = tipob;
        b->SetUserData(ud);
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
        {
            int tipo = m_b2json->getCustomInt(f, "tipo");
            sse::UserData* ud = new sse::UserData();
            ud->tipo = tipo;
            f->SetUserData(ud);
        }
    }

    debugDraw = new DebugDraw(*(m_rwindow));
    debugDraw->SetFlags(b2Draw::e_shapeBit);
    m_world->SetDebugDraw(debugDraw);

    TileSize = tilesize;
    std::vector<b2Fixture*> blocFixtures;
    m_b2json->getFixturesByName("block",blocFixtures);
    m_finder = new sse::AStarFinder(tilesize,width,height,&blocFixtures);
    return true;
}

void GameContext::DrawSysParticle()
{
    m_rwindow->draw(obj_psystem);
}

void GameContext::registerResurces()
{
    //textureKeys.add("icon_dead",thor::Resources::fromFile<sf::Texture>("assets/images/others/icon_dead.png"));
    textureKeys.add("hpbar",thor::Resources::fromFile<sf::Texture>("assets/images/others/hpbar.png"));
    textureKeys.add("blood",thor::Resources::fromFile<sf::Texture>("assets/images/particles/bloodparticle.png"));
    textureKeys.add("floor",thor::Resources::fromFile<sf::Texture>("assets/maps/area1.png"));
    textureKeys.add("target",thor::Resources::fromFile<sf::Texture>("assets/images/others/target.png"));
    textureKeys.add("pointer",thor::Resources::fromFile<sf::Texture>("assets/images/others/pointer.png"));
    textureKeys.add("rifle",thor::Resources::fromFile<sf::Texture>("assets/images/weapon/rifle.png"));
    textureKeys.add("bullet", thor::Resources::fromFile<sf::Texture>("assets/images/bullets/bullet2.png"));
}

std::shared_ptr<sf::Texture> GameContext::getPrtTexture(std::string key)
{
    try
	{
		return cache->acquire(textureKeys.get(key));
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << e.what() << std::endl;
		return 0;
	}
}

}

