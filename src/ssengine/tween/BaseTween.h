#ifndef BASETWEEN_H
#define BASETWEEN_H

#include "Easing/Easing.h"

namespace sse
{
class BaseTween
{
private:
    int repeatCnt;
    bool isIterationStep;
    bool isYoyoFlag;

    // Timings
    float repeatDelay;
    float currentTime;
    float deltaTime;
    bool isStartedFlag;     // true when the object is started
    bool isInitializedFlag; // true after the delay
    bool isFinishedFlag;    // true when all repetitions are done
    bool isKilledFlag;      // true if kill() was called
    bool isPausedFlag;      // true if pause() was called

    // Misc
    TweenCallback *callback;
    int callbackTriggers;
    void *userData;

    // Update
    void initialize();
    void testRelaunch();
    void updateStep();
    void testCompletion();

protected:
    // Timings
    float delayStart;
    float duration;

    virtual void reset();
    virtual void forceStartValues() = 0;
    virtual void forceEndValues() = 0;
    virtual void initializeOverride();
    virtual void updateOverride(int step, int lastStep, bool isIterationStep, float delta);
    virtual void forceToStart();
    virtual void forceToEnd(float time);

    void callCallback(int type);
    bool isReverse(int step);
    bool isValid(int step);

public:
    virtual ~BaseTween() {}

    virtual int getTweenCount() = 0;
    virtual int getTimelineCount() = 0;

    // Package access
    bool isAutoRemoveEnabled;
    bool isAutoStartEnabled;

    virtual BaseTween &build();
    BaseTween &start();
    BaseTween &start(TweenManager &manager);
    BaseTween &delay(float delay);
    void kill();
    virtual void free();
    void pause();
    void resume();
    BaseTween &repeat(int count, float delay);
    BaseTween &repeatYoyo(int count, float delay);
    BaseTween &setCallback(TweenCallback *callback);
    BaseTween &setCallbackTriggers(int flags);
    BaseTween &setUserData(void *data);

    // Getters
    float getDelay();
    float getDuration();
    int getRepeatCount();
    float getRepeatDelay();
    float getFullDuration();
    void *getUserData();
    int getStep();
    float getCurrentTime();
    bool isStarted();
    bool isInitialized();
    bool isFinished();
    bool isYoyo();
    bool isPaused();

    // Update
    void update(float delta);

};
}

#endif // BASETWEEN_H

