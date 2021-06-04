//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// asteroid.cpp
/// Asteroid game object.
//==============================================================================

#include "asteroid.h"
#include "blackHole.h"
#include "game.h"
#include "sound.h"
#include "jsParser.h"
#include "helper/exception.h"
#include "components/graphics.h"
#include "components/kinetics.h"

#include <numeric>


CAsteroid::CAsteroid(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color) :
    CObject{ pGame, type, pos, mass, color },
    m_maxStartSpeed{}
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
        blackHoles().front()->mass(blackHoles().front()->mass() + mass());
        game()->sound().playSound(sounds::JET, false);
    }
}

void CAsteroid::initAsteroid()
{
    //asteroid type
    float rdn{ (rand() % 100) / 100.0f};
    sprites gfx{};

    //in the future this (like all objects) should be generated automatically out of the settings
    std::vector<float> spawnChances{ game()->parser().getVFloat<parser::Debris, parser::Asteroid, parser::SpawnChance>() };
    std::vector<int> masses{ game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::Mass>() };
    std::vector<int> speeds{ game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::MaxSpeed>() };
    if (spawnChances.size() != masses.size() && masses.size() != speeds.size() && speeds.size() < 4)
        throw CException{ "Debris should have at least arrays for 4 asteroids in the settings.", INFO };
    else if (rdn < std::accumulate(spawnChances.begin(), spawnChances.begin() + 1, 0.0f))
    {
        mass(masses[0]);
        m_maxStartSpeed = speeds[0];
        gfx = sprites::ASTR_SMALL1;
    }
    else if (rdn < std::accumulate(spawnChances.begin(), spawnChances.begin() + 2, 0.0f))
    {
        mass(masses[1]);
        m_maxStartSpeed = speeds[1];
        gfx = sprites::ASTR_SMALL2;
    }
    else if (rdn < std::accumulate(spawnChances.begin(), spawnChances.begin() + 3, 0.0f))
    {
        mass(masses[2]);
        m_maxStartSpeed = speeds[2];
        gfx = sprites::ASTR_MEDIUM;
    }
    else
    {
        mass(masses[3]);
        m_maxStartSpeed = speeds[3];
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
    //average boundary
    return static_cast<float>(graphics()->width() + graphics()->height()) / 4.0f;
}
