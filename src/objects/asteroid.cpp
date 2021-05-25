//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// asteroid.cpp
/// Asteroid game object.
//==============================================================================

#include "asteroid.h"
#include "game.h"
#include "blackHole.h"
#include "parser.h"
#include "components/graphics.h"
#include "components/kinetics.h"


CAsteroid::CAsteroid(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color) :
    CObject{ pGame, type, pos, mass, color },
    m_maxStartSpeed{ pGame->parser().getInt<parser::Debris, parser::MaxSpeed>() }
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
        game()->sound()->playSound(sounds::JET, false);
    }
}

void CAsteroid::initAsteroid()
{
    //asteroid type
    int rdn{ rand() % 100 };
    sprites gfx{};

    //in the future this (like all objects) should be generated automatically out of the settings
    std::vector<int> masses{ game()->parser().getVInt<parser::Debris, parser::Mass>() };
    if (masses.size() < 4)
        throw CException{ "Debris should have at least " + std::to_string(4) + " masses in the settings.", INFO };
    else if (rdn < 35)
    {
        mass(masses[0]);
        gfx = sprites::ASTR_SMALL1;
    }
    else if (rdn < 70)
    {
        mass(masses[1]);
        gfx = sprites::ASTR_SMALL2;
    }
    else if (rdn < 95)
    {
        mass(masses[2]);
        gfx = sprites::ASTR_MEDIUM;
    }
    else
    {
        mass(masses[3]);
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
