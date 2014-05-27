#include "Bullet.h"

namespace sse
{
Bullet::Bullet(GameContext* incontext,sf::Vector2f origin, sf::Vector2f vel, float angle, std::string incode):context(incontext)
{
    code = incode;
    animatedflag = context->m_script->get<bool>(code+".animatedflag");
    texture = context->getPrtTexture("bullet");
    if(animatedflag)
    {
        cile.setSpriteSheet(*texture);
        loadFrames(&cile, code+".anim");
        animated = AnimatedSprite(sf::seconds(0.1f), true, false);
        animated.play(cile);
        animated.setRotation(angle);
    }
    else
    {
        sprite.setTexture(*texture);
        sf::Vector2f bulletcentro(sprite.getTextureRect().width / 2.f,sprite.getTextureRect().height / 2.f);
        sprite.setOrigin(bulletcentro);
        sprite.setRotation(angle);
    }

    bulletBodyDef.type = b2_dynamicBody;
    bulletB = context->m_world->CreateBody(&bulletBodyDef);
    b2CircleShape circle;
    circle.m_radius = 0.1f;
    b2FixtureDef bulletFixDef;
    bulletFixDef.shape = &circle;
    bulletFixDef.density = 5.0f;
    bulletFix = bulletB->CreateFixture(&bulletFixDef);
    //bulletFix->SetSensor(true);
    b2Filter filter;
    filter.maskBits = 65534;
    bulletFix->SetFilterData(filter);
    bulletB->SetTransform(b2Vec2(origin.x*MPP, origin.y*MPP),angle*DEGTORAD);
    bulletB->SetLinearVelocity(b2Vec2(vel.x*1.5f,-vel.y*1.5f));
    bulletB->SetUserData(this);
    udBullet = new UserData();
    udBullet->tipo = 3;
    udBullet->estado = 0;
    bulletFix->SetUserData(udBullet);
}
Bullet::~Bullet()
{
    delete(udBullet);
    context->m_world -> DestroyBody(bulletB);
}

void Bullet::draw(sf::Time delta)
{
    if(animatedflag)
    {
        animated.setPosition(bulletB->GetPosition().x*PPM, bulletB->GetPosition().y*PPM);
        animated.update(delta);
        context->m_rwindow->draw(sprite);
    }
    else
    {

        sprite.setPosition( bulletB->GetPosition().x*PPM, bulletB->GetPosition().y*PPM);
        context->m_rwindow->draw(sprite);
    }
}

void Bullet::loadFrames(Animation* anim,std::string luapath)
{
    std::vector<int> animparams = context->m_script->getIntVector(luapath);
    int nroframes = animparams[0];
    int widhtf = animparams[1];
    int heightf = animparams[2];
    int nrorow = animparams[3]*animparams[2];
    for(int i = 0 ; i < nroframes ; i++)
    {
        int val = i*widhtf;
        anim->addFrame(sf::IntRect(val, nrorow, widhtf, heightf));
    }
}

}
