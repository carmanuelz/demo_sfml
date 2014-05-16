#include "TweenManager.h"

namespace sse
{
void TweenManager::update(sf::Time frameTime)
{
    for(std::vector<Tween*>::iterator t = TweenList.begin() ; t != TweenList.end() ; t++ )
    {
        Tween* tween = *t;
        if(tween->isFinished())
        {
            TweenList.erase(t);
            t--;
        }
        else
            tween->update(frameTime);
    }
}
void TweenManager::add(Tween* tween)
{
    TweenList.push_back(tween);
}

void TweenManager::kill(Tween* tween)
{
    std::vector<Tween*>::iterator it = std::find(TweenList.begin(), TweenList.end(), tween);
    if ( it != TweenList.end() )
        TweenList.erase( it );
}
}
