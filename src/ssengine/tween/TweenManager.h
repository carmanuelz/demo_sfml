#ifndef TWEENMANAGER_H
#define TWEENMANAGER_H

#include "Tween.h"

namespace sse
{
    class TweenManager
    {
    public:
        TweenManager()
        {
        }
        void add(Tween* tween);
        void update(sf::Time frameTime);
        void kill(Tween* tween);
        void pause();
        void start();
        void killAll();
    private:
        bool ispause = false;
        std::vector<Tween*> TweenList;
    };
}

#endif // TWEENMANAGER_H
