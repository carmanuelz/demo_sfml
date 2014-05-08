#ifndef ANIMATED_ACCESSOR
#define ANIMATED_ACCESSOR

#include <SFML/Graphics.hpp>
#include "ssengine/tween/Accessor.h"
#include "ssengine/AnimatedSprite.hpp"

class AnimatedAccessor : public Accessor
{
public:
    const int POSITION = 1;
    const int ROTATION = 2;
    const int COLOR = 3;

    float* getValues(void* obj, int TweenType) const
    {
        AnimatedSprite* animated = static_cast<AnimatedSprite*>(obj);
        float* returnvalues;
        switch (TweenType)
        {
            case 1:
                returnvalues = new float[2];
                returnvalues[0] = animated->getPosition().x;
                returnvalues[1] = animated->getPosition().y;
                break;
            case 2:
                returnvalues = new float[2];
                returnvalues[0] = animated->getRotation();
                break;
            case 3:
                returnvalues = new float[4];
                sf::Color color = animated->getColor();
                returnvalues[0] = color.r;
                returnvalues[1] = color.g;
                returnvalues[2] = color.b;
                returnvalues[3] = color.a;
                break;
        }
        return returnvalues;
    }
    void setValues(void* obj, int TweenType, float* newvalues) const
    {
        AnimatedSprite* animated = static_cast<AnimatedSprite*>(obj);
        switch (TweenType)
        {
            case 1:
                animated->setPosition(newvalues[0],newvalues[1]);
                break;
            case 2:
                animated->setRotation(newvalues[0]);
                break;
            case 3:
                animated->setColor(sf::Color(newvalues[0],newvalues[1],newvalues[2],newvalues[3]));
                break;
        }
    }
};

#endif // ANIMATED_ACCESSOR
