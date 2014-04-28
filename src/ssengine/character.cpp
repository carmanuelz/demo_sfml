#include "character.h"

using namespace sse;

void saludobegin()
{
    std::cout<<"start"<<std::endl;
}
void saludoend()
{
    std::cout<<"end"<<std::endl;
}

void beginT()
{
    std::cout<<"start Transition"<<std::endl;
}
void endT()
{
    std::cout<<"end Transition"<<std::endl;
}

void Character::init()
{
    std::string path = script->get<std::string>(Code+".src");
    Cspritesheet.loadFromFile(path);
    hasweapon = script->get<bool>(Code+".hasweapon");

    /*Cstartcile.setSpriteSheet(Cspritesheet);
    loadFrames(&Cidleicle, Code+".start");*/

    Cidleicle.setSpriteSheet(Cspritesheet);
    loadFrames(&Cidleicle, Code+".idle");

	Cruncicle.setSpriteSheet(Cspritesheet);
	loadFrames(&Cruncicle, Code+".run");

    if(!hasweapon)
    {
        Catackcicle.setSpriteSheet(Cspritesheet);
        loadFrames(&Catackcicle, Code+".atack");
    }
    else
    {
        weaponspritesheet.loadFromFile("assets/rifle.png");
        waposhootcicle.setSpriteSheet(weaponspritesheet);
        loadFrames(&waposhootcicle, "w001.shoot");
        weapon = AnimatedSprite(sf::seconds(0.05f), true, false);
        weapon.setAnimation(waposhootcicle);
        weapon.play();
        weapon.setLooped(true);
    }

    offsetAnimXR = script->get<float>(Code+".offsetXR");
    offsetAnimXL = script->get<float>(Code+".offsetXL");
    offsetAnimYR = script->get<float>(Code+".offsetYR");
    offsetAnimYL = script->get<float>(Code+".offsetYL");

    animated = AnimatedSprite(sf::seconds(0.1f), true, false);
    setLeft();
    setAnimCicle(1);
    animated.setLooped(true);

    Body = createBody(x,y);

	//std::cout<<"hasta aqui todo bien";
	bulletT.loadFromFile("assets/bullet2.png");
	bulletS.setTexture(bulletT);
	sf::Vector2f bulletcentro(bulletS.getTextureRect().width / 2.f,bulletS.getTextureRect().height / 2.f);
	bulletS.setOrigin(bulletcentro);

    buffershoot.loadFromFile("assets/shoot.wav");
    soundshoot.setBuffer(buffershoot);

    /*animated.PrepareTimeLine();
    animated.PushTransition(new Transition(10,Cidleicle,saludobegin,saludoend,beginT,endT));
    animated.PushTransition(new Transition(9,Cruncicle,saludobegin,saludoend,beginT,endT));
    animated.PushTransition(new Transition(10,Cidleicle,saludobegin,saludoend,beginT,endT));
    animated.StartTimeLine();*/
}

void Character::loadFrames(Animation* anim,std::string luapath)
{
    std::vector<int> animparams = script->getIntVector(luapath);
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
/*  0 = start
    1 = idle
    2 = run
    3 = atack
    4 = hide
*/
void Character::setAnimCicle(int codecicle)
{
    switch(codecicle)
    {
    case 0:
        animated.play(Cstartcile);
        break;
    case 1:
        animated.play(Cidleicle);
        break;
    case 2:
        animated.play(Cruncicle);
        break;
    case 3:
        animated.play(Catackcicle);
        break;
    case 4:
        animated.play(ChideIcle);
        break;
    }
}

void Character::setLeft()
{
    animated.FlipX();
    ofsetanimx = offsetAnimXL;
}

void Character::setRight()
{
    animated.unFlipX();
    ofsetanimx = offsetAnimXR;
}

b2Body* Character::createBody(float x, float y)
{
    b2BodyDef BodyDef;
    BodyDef.type = b2_dynamicBody;
	b2FixtureDef bodyfixtureDef;
    b2CircleShape circleshape;
    circleshape.m_radius = 0.2;
    bodyfixtureDef.shape = &circleshape;
    b2Filter filter;
	b2Body* characterbody = world->CreateBody(&BodyDef);
    if(Type != 1)
    {
        BodyDef.linearDamping = 20;
        filter.categoryBits = 3;
    }
    bodyfixtureDef.friction = 0;
	b2Fixture* BodyFix = characterbody->CreateFixture(&bodyfixtureDef);
    BodyFix->SetFilterData(filter);
	characterbody->SetBullet(false);
	characterbody->SetTransform(b2Vec2(x*MPP, y*MPP),0);
	characterbody->SetFixedRotation(true);
	UserData* ud = new UserData();
    ud->tipo = Type;
    ud->estado = 0;
    BodyFix->SetUserData(ud);
    characterbody->SetUserData(this);
    vel.x = 0;
    vel.y = 0;
	return characterbody;
}

void Character::update(sf::Time frameTime)
{
    if(Acumulator < 100)
        Acumulator+=frameTime.asSeconds();
    b2Vec2 position = Body->GetPosition();
    x = position.x*PPM;
    y = position.y*PPM;
    animated.update(frameTime);
    animated.setPosition(x + ofsetanimx, y + offsetAnimYR);
    if(hasweapon)
    {
        weapon.setPosition(x + ofsetanimx+CgunOffset.x, y + offsetAnimYR + CgunOffset.y);
        weapon.update(frameTime);
    }
}

void Character::updatebehaviour(float TargetX,float TargetY)
{
    sf::Vector2f dif(TargetX - 24.0f - animated.getPosition().x,(TargetY - 39.0f - animated.getPosition().y)*-1);
    angle = fmod(atan2(dif.y, dif.x)*180/M_PI,360);
    float module = sqrt(pow(dif.x,2)+pow(dif.y,2));
    bulletVU = sf::Vector2f(dif.x/module,dif.y/module);
    bulletOrigin = sf::Vector2f(animated.getPosition().x+bulletVU.x*30 + 24, animated.getPosition().y-bulletVU.y*30+39);
    if(angle < 0 )
        angle = 360 + angle;
    angle = 360 - angle;

    if(angle < 360 && angle >= 270)
    {
        direction = 1;
        weapon.unFlipX();
        CgunOffset.x = 21;
        CgunOffset.y = 30;
        weapon.setOrigin(0,0);
        weapon.rotate(angle - weapon.getRotation());
        before = true;
    }
    if(angle < 270 && angle >= 180)
    {
        direction = 2;
        weapon.FlipX();
        CgunOffset.x = 27;
        CgunOffset.y = 30;
        weapon.setOrigin(36,0);
        weapon.rotate(-weapon.getRotation());
        weapon.rotate(angle - 180 - weapon.getRotation());
        before = true;
    }
    if(angle < 180 && angle >= 90)
    {
        direction = 2;
        weapon.FlipX();
        CgunOffset.x = 18;
        CgunOffset.y = 40;
        weapon.setOrigin(27,8);
        weapon.rotate(-weapon.getRotation());
        weapon.rotate(angle - 180 - weapon.getRotation());
        before = false;
    }

    if(angle < 90 && angle >= 0)
    {
        direction = 1;
        weapon.unFlipX();
        CgunOffset.x = 28;
        CgunOffset.y = 40;
        weapon.setOrigin(8,8);
        weapon.rotate(angle - weapon.getRotation());
        before = false;
    }
}

b2Body* Character::createBullet(sf::Vector2f origin, sf::Vector2f vel,float angle)
{
    b2BodyDef bulletBodyDef;
    bulletBodyDef.type = b2_dynamicBody;
	b2Body* bulletB = world->CreateBody(&bulletBodyDef);
	b2CircleShape circle;
	circle.m_radius = 0.1f;
	b2FixtureDef bulletFixDef;
	bulletFixDef.shape = &circle;
	bulletFixDef.density = 0.0f;
	b2Fixture* bulletFix = bulletB->CreateFixture(&bulletFixDef);
	b2Filter filter;
	filter.maskBits = 65534;
	bulletFix->SetFilterData(filter);
	bulletB->SetTransform(b2Vec2(origin.x*MPP, origin.y*MPP),angle*DEGTORAD);
	bulletB->SetLinearVelocity(b2Vec2(vel.x*1.5f,-vel.y*1.5f));
	UserData* udBullet = new UserData();
    udBullet->tipo = 3;
    udBullet->estado = 0;
    bulletFix->SetUserData(udBullet);
    BulletList->push_back(bulletB);
    bulletB->SetUserData(&bulletS);
	return bulletB;
}

/* AI Character*/
void AICharacter::setpathfinding(AStarFinder* inAStart, float intileSize)
{
    AStartF = inAStart;
    TileSize = intileSize;
	preenemmyX = floor(x/TileSize);
	preenemmyY = floor(y/TileSize);
}

void AICharacter::update(sf::Time frameTime)
{
    Body->SetLinearVelocity(vel);
    Character::update(frameTime);
}

void AICharacter::updateFind()
{
    if(Target != 0)
    {
        b2Vec2 destination = Target->GetPosition();
        destination.x*=PPM;
        destination.y*=PPM;
        findto(destination.x,destination.y);
    }
}

void AICharacter::setTarget(b2Body* inTarget)
{
    Target = inTarget;
}

void AICharacter::findto(float tox, float toy)
{
    int endX = floor(tox/TileSize);
    int endY = floor(toy/TileSize);
    float floatX = x/TileSize;
    float floatY = y/TileSize;
    int startX;
    int startY;
    if((floatX - preenemmyX)>1.5f || (preenemmyX - floatX)>0.5f)
    {
        startX = floor(floatX);
        preenemmyX = startX;
    }
    else
    {
        startX = preenemmyX;
    }

    if((floatY - preenemmyY)>1.5f || (preenemmyY - floatY)>0.5f)
    {
        startY = floor(floatY);
        preenemmyY = startY;
    }
    else
    {
        startY = preenemmyY;
    }

    std::vector<sf::Vector2i*> route;
    if(AStartF->isWalkableAt(startX,startY) && AStartF->isWalkableAt(endX,endY))
    {
        route = AStartF->findPath(startX,startY,endX,endY);
        int len = route.size();
        if(len>1)
        {
            sf::Vector2f difstep((route[len-2]->x - startX)*TileSize, (route[len-2]->y - startY)*TileSize);

            float modenemy = sqrt(pow(difstep.x,2)+pow(difstep.y,2));

            if(modenemy != 0)
            {
                sf::Vector2f enemmyVU((difstep.x)/modenemy,(difstep.y)/modenemy);
                b2Vec2 enemyVel(enemmyVU.x*facVel,enemmyVU.y*facVel);
                vel = enemyVel;
                if(enemyVel.x > 0)
                    setRight();
                else
                {
                    if(enemyVel.x < 0)
                        setLeft();
                    else
                    {
                        if(tox < x)
                            setLeft();

                        else
                            setRight();

                    }
                }
            }
        }
    }
}

/*player*/

void Player::update(sf::Time frameTime)
{
    Body->SetLinearVelocity(vel);
    Character::update(frameTime);
}

b2Vec2 Player::updatePlayer()
{
    Body->SetLinearVelocity(b2Vec2(0,0));
    vel = Body->GetLinearVelocity();
    bool isrun = false;

    setRight();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)||
       sf::Keyboard::isKeyPressed(sf::Keyboard::D)||
       sf::Keyboard::isKeyPressed(sf::Keyboard::W)||
       sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        setAnimCicle(2);
        isrun = true;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            vel.x = -SPEED;
            if(direction == 1)
            {
                setRight();
                animated.setReverse(true);
            }

            else
            {
                setLeft();
                animated.setReverse(false);
            }

        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            vel.x = SPEED;
            if(direction == 1)
            {
                setRight();
                animated.setReverse(false);
            }
            else
            {
                setLeft();
                animated.setReverse(true);
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            vel.y = -SPEED;
            animated.setReverse(false);
            if(direction == 1)
                setRight();
            else
                setLeft();
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            animated.setReverse(false);
            vel.y = SPEED;
            if(direction == 1)
                setRight();
            else
                setLeft();
        }

    }

    if(!isrun)
    {
       setAnimCicle(1);
        if(direction == 1)
            setRight();
        else
            setLeft();
    }

    if(vel.x != 0 && vel.y!=0)
    {
        vel.x = vel.x*0.7;
        vel.y = vel.y*0.7;
    }

    weapon.play();
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        weapon.stop();
    }

    else
    {
        if(Acumulator >= 0.2)
        {
            createBullet(bulletOrigin, bulletVU, angle);
            Acumulator = 0;
            soundshoot.play();
        }
    }

    return b2Vec2(x,y);
}


