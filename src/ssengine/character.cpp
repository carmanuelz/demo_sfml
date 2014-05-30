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

Character::~Character()
{
    context->m_tweenmanager->kill(&tween);
    context->m_world -> DestroyBody(Body);
}

void Character::init()
{
    std::string path = context->m_script->get<std::string>(Code+".src");
    Cspritesheet.loadFromFile(path);
    hasweapon = context->m_script->get<bool>(Code+".hasweapon");
    HP = context->m_script->get<float>(Code+".HP");
    Damage = context->m_script->get<float>(Code+".damage");
    Type = context->m_script->get<int>(Code+".type");

    /*Cstartcile.setSpriteSheet(Cspritesheet);
    loadFrames(&Cidleicle, Code+".start");*/

    Cidlecile.setSpriteSheet(Cspritesheet);
    loadFrames(&Cidlecile, Code+".idle");

    Cruncicle.setSpriteSheet(Cspritesheet);
    loadFrames(&Cruncicle, Code+".run");

    if(Type == 1)
    {
        Cdeadcile.setSpriteSheet(Cspritesheet);
        loadFrames(&Cdeadcile, Code+".dead");
    }

    if(!hasweapon)
    {
        Cattackcicle.setSpriteSheet(Cspritesheet);
        loadFrames(&Cattackcicle, Code+".attack");
    }
    else
    {
        wspritesheet = context->getPrtTexture("rifle");
        waposhootcicle.setSpriteSheet(*wspritesheet);
        loadFrames(&waposhootcicle, "w001.shoot");
        weapon = AnimatedSprite(sf::seconds(0.05f), true, false);
        weapon.setAnimation(waposhootcicle);
        //weapon.setLooped(true);
        weapon.play();
    }

    offsetAnimXR = context->m_script->get<float>(Code+".offsetXR");
    offsetAnimXL = context->m_script->get<float>(Code+".offsetXL");
    offsetAnimYR = context->m_script->get<float>(Code+".offsetYR");
    offsetAnimYL = context->m_script->get<float>(Code+".offsetYL");

    animated = AnimatedSprite(sf::seconds(0.1f), true, false);
    setLeft();
    setAnimCicle(1);
    animated.setLooped(true);
    //animated.setColor(sf::Color(255,0,0));

    Body = createBody(x,y);

    buffershoot.loadFromFile("assets/sounds/shoot.wav");
    soundshoot.setBuffer(buffershoot);

    animaccess = new AnimatedAccessor();
    float* values= new float[3];
    values[0] = 255;
    values[1] = 0;
    values[2] = 0;

    options = new sse::tweenOptions();
    options->target = &(animated);
    options->accessor = animaccess;
    options->easefunc = sse::easing::easeInLinear;
    options->tweentype = animaccess->COLOR;
    options->destination = values;
    options->sizevalue = 3;
    options->yoyo = true;
    options->repeatCnt = 0;
    options->timeCicle = 0.2f;
    tween.to(options);
}

void Character::loadFrames(Animation* anim,std::string luapath)
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
        animated.play(Cidlecile);
        break;
    case 2:
        animated.play(Cruncicle);
        break;
    case 3:
        animated.play(Cattackcicle);
        break;
    case 4:
        animated.play(ChideIcle);
        break;
    case 5:
        animated.play(Cdeadcile);
        break;
    }
}

void Character::setLeft()
{
    animated.FlipX();
    ofsetanimx = offsetAnimXL;
    isright = false;
}

void Character::setRight()
{
    animated.unFlipX();
    ofsetanimx = offsetAnimXR;
    isright = true;
}

b2Body* Character::createBody(float x, float y)
{
    UserData* ud = new UserData();
    ud->tipo = Type;
    ud->estado = 0;

    b2BodyDef BodyDef;
    BodyDef.type = b2_dynamicBody;
    b2FixtureDef bodyfixtureDef;

    b2CircleShape circleshape;
    circleshape.m_radius = 0.22;

    b2PolygonShape poligonshape;
    poligonshape.SetAsBox(0.22, 0.33);

    b2Filter filter;
    b2Body* characterbody = context->m_world->CreateBody(&BodyDef);
    bodyfixtureDef.friction = 0;

    b2FixtureDef sensordef;
    b2CircleShape sensorshape;
    b2Fixture* sensor;
    bodyfixtureDef.shape = &circleshape;
    if(Type == 1)
    {
        circleshape.m_radius = 0.25;
        sensorshape.m_radius = 0.37;
        sensordef.shape = &sensorshape;
        sensor = characterbody->CreateFixture(&sensordef);
        sensor->SetUserData(ud);
        sensor->SetSensor(true);
        filter.maskBits = 65533;
        filter.categoryBits = 5;
        sensor->SetFilterData(filter);
    }
    else
    {
        circleshape.m_radius = 0.18;
        sensorshape.m_radius = 0.37;
        sensordef.shape = &sensorshape;
        sensor = characterbody->CreateFixture(&sensordef);
        sensor->SetUserData(ud);
        sensor->SetSensor(true);
        BodyDef.linearDamping = 20;
        filter.maskBits = 65534;
        filter.categoryBits = 4;
        sensor->SetFilterData(filter);
    }
    b2Fixture* BodyFix = characterbody->CreateFixture(&bodyfixtureDef);
    BodyFix->SetFilterData(filter);
    characterbody->SetBullet(false);
    characterbody->SetTransform(b2Vec2(x*MPP, y*MPP),0);
    characterbody->SetFixedRotation(true);
    BodyFix->SetUserData(ud);
    characterbody->SetUserData(this);
    vel.x = 0;
    vel.y = 0;
    return characterbody;
}

void Character::update(sf::Time frameTime)
{
    Body->SetLinearVelocity(vel);
    if(Acumulator < 100)
        Acumulator+=frameTime.asSeconds();

    if(Target != 0)
        currenttimefind+=frameTime.asSeconds();

    b2Vec2 position = Body->GetPosition();
    x = position.x*PPM;
    y = position.y*PPM;
    animated.update(frameTime);
    animated.setPosition(x + ofsetanimx, y + offsetAnimYR);

    TestCollision();
    if(hasweapon)
    {
        weapon.setPosition(x + ofsetanimx+CgunOffset.x, y + offsetAnimYR + CgunOffset.y);
        weapon.update(frameTime);
    }
    if(isbusy)
       if(!animated.isTimeLine)
            isbusy = false;

    if(shootbusy)
        if(Acumulator > 1.5f)
        {
            shootbusy = false;
        }

    if(Type == 1 && !isdeadflag)
    {
        if(HP <= 0)
        {
            vel = b2Vec2(0,0);
            setAnimCicle(5);
            animated.setReverse(false);
            animated.setLooped(false);
            isdeadflag = true;
            hasweapon = false;
            blockbehaviour = true;
        }
    }
}

void Character::takeDamage(float inDamage)
{
    HP -= inDamage;
}

float Character::Hit()
{
    currenttimefind = 0;
    return Damage;
}

float Character::getHP()
{
    return HP;
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
        weapon.rotate(angle - 179 - weapon.getRotation());
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

    weapon.refresh();

}

b2Body* Character::createBullet(sf::Vector2f origin, sf::Vector2f bvel,float angle)
{
    bool shootflag = true;
    if (Type != 1)
        shootflag = false;
    BulletList->push_back(new Bullet(context,origin,bvel,angle,"g001",shootflag,this));
}

void Character::addCollisionList(b2Fixture*f)
{
    CollisionList.push_back(f);
}

void Character::removeCollisionList(b2Fixture*f)
{
    std::vector<b2Fixture*>::iterator it = std::find(CollisionList.begin(), CollisionList.end(), f);
    if ( it != CollisionList.end() )
        CollisionList.erase( it );
}

void Character::TestCollision()
{
    for(auto k = CollisionList.cbegin() ; k != CollisionList.cend() ; k++ )
    {
        b2Fixture* f = *k;
        sse::UserData* userdata = static_cast<sse::UserData*>(f->GetUserData());
        switch (userdata->tipo)
        {
        case objectType::obj_typePlayer:
            if(!isbusy)
            {
                if(!hasweapon)
                {
                    animated.PrepareTimeLine();
                    animated.PushTransition(new Transition(1,Cattackcicle ,0,0,0,0));
                    animated.PushTransition(new Transition(1,Cruncicle,0,0,0,0));
                    animated.StartTimeLine();
                    isbusy = true;
                    b2Vec2 pos = f->GetBody()->GetPosition();
                    context->m_psystem->addEmitter(BloodEmitter(sf::Vector2f(pos.x*PPM,pos.y*PPM)), sf::seconds(0.1f));
                    Character* player = static_cast<sse::Character*>(f->GetBody()->GetUserData());
                    player->takeDamage(Hit());
                }
            }
            break;
        case objectType::obj_typeBullet:
            Bullet* bullet = static_cast<sse::Bullet*>(f->GetBody()->GetUserData());
            if(bullet->estado == 0)
            {
                context->m_psystem->addEmitter(BloodEmitter(sf::Vector2f(x,y)), sf::seconds(0.1f));
                bullet->estado = 1;
                takeDamage(5);
                if(Type == objectType::obj_typeEnemy)
                    if(Target == 0)
                    {
                        setAnimCicle(2);
                        Character* player = static_cast<sse::Character*>(bullet->fromcharacter);
                        Target = player->Body;
                    }
            }
            break;
        }

        if(HP<15 && finished == false)
        {
            context->m_tweenmanager->add(&tween);
            finished = true;
        }
        if(HP <= 0 && Type != 1)
            needremove = true;
    }
}



