#ifndef TWEEN_H
#define TWEEN_H

#include <SFML/System.hpp>
#include <iostream>
#include "Easing/Easing.h"
#include "Accessor.h"

namespace sse
{
    class Tween
    {
    public:
        Tween():tween(this){}
        void to(Accessor* inaccessor, void *intarget, int intweentype, float (* ineasefunc) (float t,float b , float c, float d), float* into, float intotaltime, int insizevalue);
        void from(Accessor* inaccessor, void *intarget, int intweentype, float (* ineasefunc) (float t,float b , float c, float d), float* infrom, float intotaltime, int insizevalue);
        void update(sf::Time frameTime);
        void resstart();
        void stop();
    private:
        Accessor* accessor;
        Tween* tween;
        int tweentype;
        float* destination;
        float* origin;
        float* currentvalues;
        float* initialvalues;
        float timecounter;
        float totaltime;
        int repeatCnt;
        bool isIterationStep;
        bool isYoyoFlag;
        int valuesize;

        // Timings
        float repeatDelay;
        float currentTime;
        float deltaTime;
        bool isStartedFlag;     // true when the object is started
        bool isInitializedFlag; // true after the delay
        bool isFinishedFlag;    // true when all repetitions are done
        bool isKilledFlag;      // true if kill() was called
        bool isPausedFlag;      // true if pause() was called

        void *target;
        float (*easefunc)(float t,float b , float c, float d);
    };
}

#endif
