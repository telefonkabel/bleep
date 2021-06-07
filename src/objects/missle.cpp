//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// missle.cpp
/// Missle game object.
//==============================================================================

#include "missle.h"
#include "components/graphics.h"
#include "components/kinetics.h"


CMissle::CMissle(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color) :
    CObject{ pGame, type, pos, mass, color }
{
}

CMissle::~CMissle()
{
}

void CMissle::update(float deltaTime)
{
    ;
}

float CMissle::edge()
{
    return 0.0f;
}