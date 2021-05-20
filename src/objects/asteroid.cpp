//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// asteroid.cpp
/// Asteroid game object.
//==============================================================================

#include "components/graphics.h"
#include "components/kinetics.h"
#include "asteroid.h"
#include "game.h"
#include "player.h"


CAsteroid::CAsteroid(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color) :
    CObject{ pGame, type, pos, mass, color },
    m_maxStartSpeed{ 200 }
{
	addComponent(components::kinetics);
	addComponent(components::graphics);

	initAsteroid();
     
	eatable(true);
}

CAsteroid::~CAsteroid()
{
    if (state() == objectStates::EATEN)
    {
        player()->mass(player()->mass() + mass());
        game()->sound()->playSound(sounds::JET, false);
    }
}

void CAsteroid::initAsteroid()
{
    //asteroid type
    int rdn{ rand() % 100 };
    sprites gfx{};
    if (rdn < 35)
    {
        mass(10000);
        gfx = sprites::ASTR_SMALL1;
    }
    else if (rdn < 70)
    {
        mass(10000);
        gfx = sprites::ASTR_SMALL2;
    }
    else if (rdn < 95)
    {
        mass(20000);
        gfx = sprites::ASTR_MEDIUM;
    }
    else
    {
        mass(40000);
        gfx = sprites::ASTR_BIG;
    }
    graphics()->sprite(objectTypes::DEBRIS, gfx);

    //start point
    xy(maths::rndCirclePt(game()->center(), static_cast<float>(game()->radiusMap())));
    //start velocity
    float rndAngle{ maths::rndAngle() };
    int startSpeed{ rand() % m_maxStartSpeed };
    kinetics()->velocity(v2d{ startSpeed * static_cast<float>(cos(rndAngle)), startSpeed * static_cast<float>(sin(rndAngle)) });
}

void CAsteroid::update(float deltaTime)
{
	CObject::update(deltaTime);
}

float CAsteroid::edge()
{
    return static_cast<float>(graphics()->width() + graphics()->height()) / 4.0f;
}
