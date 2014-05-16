#ifndef TWEEN_H
#define TWEEN_H

#include <SFML/System.hpp>
#include <iostream>
#include "Easing/Easing.h"
#include "Accessor.h"

namespace sse
{
struct tweenOptions
{
    tweenOptions()
    {
        repeatCnt = 1;
        yoyo = false;
    }
    void *target;
    Accessor* accessor;
    float (* easefunc) (float t,float b , float c, float d);
    int tweentype;
    float* destination;
    float timeCicle;
    int sizevalue;
    bool yoyo;
    int repeatCnt;
};

class Tween
{
public:
    Tween():tween(this) {}
    void to(tweenOptions* options);
    void from(tweenOptions* options);
    void update(sf::Time frameTime);
    void resstart();
    void stop();
    bool isFinished();
private:
    Accessor* accessor;
    Tween* tween;
    int tweentype;
    float* destination;
    float* origin;
    float* currentvalues;
    float* initialvalues;
    float timecounter;
    float timeCicle;
    int repeatCnt;
    int repeatCount;
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
    bool isReverse;

    void *target;
    float (*easefunc)(float t,float b , float c, float d);
    void updateCicle();
    void prepareCicle(float delta);
};
}

#endif
