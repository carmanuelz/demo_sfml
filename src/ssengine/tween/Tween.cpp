#include "Tween.h"
namespace sse
{
void Tween::to(tweenOptions* options)
{
    target = options->target;
    accessor = options->accessor;
    easefunc = options->easefunc;
    tweentype = options->tweentype;
    destination = options->destination;
    valuesize = options->sizevalue;
    isYoyoFlag = options->yoyo;

    repeatCnt = options->repeatCnt;
    currentvalues = accessor->getValues(target,tweentype);
    initialvalues = accessor->getValues(target,tweentype);
    timeCicle = options->timeCicle;
    isReverse = false;
    isFinishedFlag = false;
    repeatCount = 0;
    timecounter = 0;
}

bool Tween::isFinished()
{
    return isFinishedFlag;
}

void Tween::update(sf::Time frameTime)
{
    if(repeatCount < repeatCnt || repeatCnt == 0)
    {
        prepareCicle(frameTime.asSeconds());
    }
    else
        isFinishedFlag = true;
}

void Tween::prepareCicle(float delta)
{
    if(!isReverse)
    {
        if(timecounter + delta < timeCicle)
        {
            timecounter += delta;
        }
        else
        {
            timecounter = timeCicle;
            isReverse = true;
        }
        updateCicle();
    }
    else
    {
        if(isYoyoFlag)
        {
            if(timecounter - delta > 0)
            {
                timecounter -= delta;
            }
            else
            {
                timecounter = 0;
                isReverse = false;
                repeatCount++;
            }
            updateCicle();
        }
        else
            repeatCount++;
    }
}

void Tween::updateCicle()
{
    for(int i = 0; i < valuesize; i++)
    {
        currentvalues[i] = easefunc(timecounter, initialvalues[i],destination[i]-initialvalues[i],timeCicle);
    }
    accessor->setValues(target,tweentype,currentvalues);
}
}

