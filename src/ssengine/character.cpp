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

    Cidleicle.setSpriteSheet(Cspritesheet);
    loadFrames(&Cidleicle, Code+".idle");

    Cruncicle.setSpriteSheet(Cspritesheet);
    loadFrames(&Cruncicle, Code+".run");

    if(!hasweapon)
    {
        Cattackcicle.setSpriteSheet(Cspritesheet);
        loadFrames(&Cattackcicle, Code+".attack");
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

    //std::cout<<"hasta aqui todo bien";
    bulletT.loadFromFile("assets/bullet2.png");
    bulletS.setTexture(bulletT);
    sf::Vector2f bulletcentro(bulletS.getTextureRect().width / 2.f,bulletS.getTextureRect().height / 2.f);
    bulletS.setOrigin(bulletcentro);

    buffershoot.loadFromFile("assets/shoot.wav");
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
        animated.play(Cidleicle);
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

    b2FixtureDef sensorPLayerdef;
    b2CircleShape sensorshape;
    b2Fixture* sensorPlayer;
    if(Type == 1)
    {

        bodyfixtureDef.shape = &poligonshape;
        sensorshape.m_radius = 0.37;
        sensorPLayerdef.shape = &sensorshape;
        sensorPlayer = characterbody->CreateFixture(&sensorPLayerdef);
        sensorPlayer->SetUserData(ud);
        sensorPlayer->SetSensor(true);
    }
    else
    {

        bodyfixtureDef.shape = &circleshape;
        circleshape.m_radius = 0.22;
        BodyDef.linearDamping = 20;
        filter.categoryBits = 3;
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
    TestCollision();
    if(Acumulator < 100)
        Acumulator+=frameTime.asSeconds();

    if(Target != 0)
        currenttimefind+=frameTime.asSeconds();

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
    if(isbusy)
        if(!animated.isTimeLine)
            isbusy = false;
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

sf::Vector2f Character::updatebehaviour(float TargetX,float TargetY)
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

    return bulletVU;
}

b2Body* Character::createBullet(sf::Vector2f origin, sf::Vector2f vel,float angle)
{
    b2BodyDef bulletBodyDef;
    bulletBodyDef.type = b2_dynamicBody;
    b2Body* bulletB = context->m_world->CreateBody(&bulletBodyDef);
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
    context->BulletList.push_back(bulletB);
    bulletB->SetUserData(&bulletS);
    return bulletB;
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
            break;
        case objectType::obj_typeBullet:
            context->m_psystem->addEmitter(BloodEmitter(sf::Vector2f(x,y)), sf::seconds(0.1f));
            if(userdata->estado == 0)
                context->RemoveList.push_back(f->GetBody());
            userdata->estado=1;
            takeDamage(5);
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



