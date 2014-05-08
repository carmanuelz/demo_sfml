#ifndef ACCESSOR_H
#define ACCESSOR_H

class Accessor
{
public:
    virtual float* getValues(void* obj,int TweenType) const = 0;
    virtual void setValues(void* obj,int TweenType, float* newvalues) const = 0;
};

#endif
