#ifndef SSENGINE_H
#define SSENGINE_H

#include "AStarFinder.h"
#include "AnimatedSprite.hpp"
#include "loadconf/LuaScript.h"
#include "particlesys.h"
#include "util.h"
#include "tween/Tween.h"
#include "AICharacter.h"
#include "Player.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define TIMESTEP 1.0f/60.0f     //TIEMPO DE REFRESCO
#define VELITER 10              //NUMERO DE ITERACION POR TICK PARA CALCULAR LA VELOCIDAD
#define POSITER 10              //NUMERO DE ITERACIONES POR TICK PARA CALCULAR LA POSICION

#define PPM 64.0f               //PIXELS POR METRO
#define MPP (1.0f/PPM)          //METROS POR PIXEL
#define M_PI 3.14159265358979323846

namespace sse
{
}

#endif
