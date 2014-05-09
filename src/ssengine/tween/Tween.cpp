#include "Tween.h"
namespace sse
{
    void Tween::to(Accessor* inaccessor, void *intarget,int intweentype, float (* ineasefunc) (float t,float b , float c, float d), float* into, float intotaltime, int insizevalue)
    {
        target = intarget;
        accessor = inaccessor;
        easefunc = ineasefunc;
        tweentype = intweentype;
        destination =  into;
        timecounter = 0;
        totaltime = intotaltime;
        currentvalues = inaccessor->getValues(target,tweentype);
        initialvalues = inaccessor->getValues(target,tweentype);
        valuesize = insizevalue;
    }

    void Tween::update(sf::Time frameTime)
    {
        if(timecounter < totaltime)
        {
            for(int i = 0; i < valuesize; i++)
            {
                currentvalues[i] = easefunc(timecounter, initialvalues[i],destination[i]-initialvalues[i],totaltime);
            }
            accessor->setValues(target,tweentype,currentvalues);
            timecounter += frameTime.asSeconds();
            std::cout<<"color: ("<<currentvalues[0]<<","<<currentvalues[1]<<","<<currentvalues[2]<<")"<<std::endl;
        }
    }
}

