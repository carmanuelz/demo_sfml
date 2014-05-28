#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include "GameContext.h"
#include "Bullet.h"

namespace sse
{
class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        b2Fixture* FixtA = contact->GetFixtureA();
        b2Fixture* FixtB = contact->GetFixtureB();
        sse::UserData* userdataA = static_cast<sse::UserData*>(FixtA->GetUserData());
        sse::UserData* userdataB = static_cast<sse::UserData*>(FixtB->GetUserData());
        if(userdataA->tipo == 3 && userdataA->tipo == 2 && userdataA->estado==0)
        {
            sse::Bullet* bullet = static_cast<sse::Bullet*>(FixtA->GetBody()->GetUserData());
            if(bullet->estado == 0)
            {
                bullet->estado = 1;
            }
        }
        else if(userdataB->tipo == 3 && userdataA->tipo == 2 && userdataB->estado==0)
        {
            sse::Bullet* bullet = static_cast<sse::Bullet*>(FixtB->GetBody()->GetUserData());
            if(bullet->estado == 0)
            {
                bullet->estado = 1;
            }
        }

        if(userdataA->tipo == 4 || userdataA->tipo == 1)
        {
            b2Body* bodyA = FixtA->GetBody();
            sse::Character* collCharacter = static_cast<sse::Character*>(bodyA->GetUserData());
            collCharacter->addCollisionList(FixtB);
        }
        else if(userdataB->tipo == 4 || userdataA->tipo == 1)
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
        else if(userdataB->tipo == 4)
        {
            b2Body* bodyB = FixtB->GetBody();
            sse::Character* collCharacter = static_cast<sse::Character*>(bodyB->GetUserData());
            collCharacter->removeCollisionList(FixtA);
        }
    }

public:
    ContactListener(GameContext* incontext):context(incontext) {}
private:
    GameContext* context;
};
}
#endif // CONTACTLISTENER_H
