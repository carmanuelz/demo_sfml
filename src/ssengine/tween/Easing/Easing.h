#ifndef EASING_H
#define EASING_H

#include "Back.h"
#include "Bounce.h"
#include "Circ.h"
#include "Cubic.h"
#include "Elastic.h"
#include "Expo.h"
#include "Linear.h"
#include "Quad.h"
#include "Quart.h"
#include "Quint.h"
#include "Sine.h"


namespace sse
{
namespace easing
{
static float (* easeInQuad) (float t,float b , float c, float d) = (new Quad())->easeIn ;
static float (* easeOutQuad) (float t,float b , float c, float d) = (new Quad())->easeOut ;
static float (* easeInOutQuad) (float t,float b , float c, float d) = (new Quad())->easeInOut ;

static float (* easeInLinear) (float t,float b , float c, float d) = (new Linear())->easeIn ;
static float (* easeOutLinear) (float t,float b , float c, float d) = (new Linear())->easeOut ;
static float (* easeInOutLinear) (float t,float b , float c, float d) = (new Linear())->easeInOut ;

static float (* easeInBack) (float t,float b , float c, float d) = (new Back())->easeIn ;
static float (* easeOutBack) (float t,float b , float c, float d) = (new Back())->easeOut ;
static float (* easeInOutBack) (float t,float b , float c, float d) = (new Back())->easeInOut ;

static float (* easeInBounce) (float t,float b , float c, float d) = (new Bounce())->easeIn ;
static float (* easeOutBounce) (float t,float b , float c, float d) = (new Bounce())->easeOut ;
static float (* easeInOutBounce) (float t,float b , float c, float d) = (new Bounce())->easeInOut ;

static float (* easeInCirc) (float t,float b , float c, float d) = (new Circ())->easeIn ;
static float (* easeOutCirc) (float t,float b , float c, float d) = (new Circ())->easeOut ;
static float (* easeInOutCirc) (float t,float b , float c, float d) = (new Circ())->easeInOut ;

static float (* easeInCubic) (float t,float b , float c, float d) = (new Cubic())->easeIn ;
static float (* easeOutCubic) (float t,float b , float c, float d) = (new Cubic())->easeOut ;
static float (* easeInOutCubic) (float t,float b , float c, float d) = (new Cubic())->easeInOut ;

static float (* easeInElastic) (float t,float b , float c, float d) = (new Elastic())->easeIn ;
static float (* easeOutElastic) (float t,float b , float c, float d) = (new Elastic())->easeOut ;
static float (* easeInOutElastic) (float t,float b , float c, float d) = (new Elastic())->easeInOut ;

static float (* easeInExpo) (float t,float b , float c, float d) = (new Expo())->easeIn ;
static float (* easeOutExpo) (float t,float b , float c, float d) = (new Expo())->easeOut ;
static float (* easeInOutExpo) (float t,float b , float c, float d) = (new Expo())->easeInOut ;

static float (* easeInQuart) (float t,float b , float c, float d) = (new Quart())->easeIn ;
static float (* easeOutQuart) (float t,float b , float c, float d) = (new Quart())->easeOut ;
static float (* easeInOutQuart) (float t,float b , float c, float d) = (new Quart())->easeInOut ;

static float (* easeInQuint) (float t,float b , float c, float d) = (new Quint())->easeIn ;
static float (* easeOutQuint) (float t,float b , float c, float d) = (new Quint())->easeOut ;
static float (* easeInOutQuint) (float t,float b , float c, float d) = (new Quint())->easeInOut ;

static float (* easeInSine) (float t,float b , float c, float d) = (new Sine())->easeIn ;
static float (* easeOutSine) (float t,float b , float c, float d) = (new Sine())->easeOut ;
static float (* easeInOutSine) (float t,float b , float c, float d) = (new Sine())->easeInOut ;
}
}

#endif // EASING_H

