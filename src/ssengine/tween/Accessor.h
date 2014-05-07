#ifndef ACCESSOR_H
#define ACCESSOR_H

#include "Easing/Easing.h"

namespace sse
{
    template<class T>
    class Accessor
    {
    public:
        virtual float getValues(T* obj,int TweenType);
        virtual void setValues(T* obj,int TweenType, float* newvalues);
    };
}


#endif
