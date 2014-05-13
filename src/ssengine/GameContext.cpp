#include "GameContext.h"

namespace sse
{
    GameContext::GameContext()
    {
        //ctor
    }

    GameContext::~GameContext()
    {
        //dtor
    }

    bool GameContext::LoadWorld(const char* filename)
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
        return true;
    }
    void GameContext::Createfinder(float tilesize, int width, int height)
    {
        std::vector<b2Fixture*> blocFixtures;
        m_b2json->getFixturesByName("block",blocFixtures);
        m_finder = new sse::AStarFinder(tilesize,width,height,&blocFixtures);
    }

    void GameContext::DrawSysParticle()
    {
        m_rwindow->draw(obj_psystem);
    }
}

