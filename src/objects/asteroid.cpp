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
    //load settings
    std::vector<int> spawnChances{ game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::SpawnChance>() };
    std::vector<int> masses{ game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::Mass>() };
    std::vector<int> speeds{ game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::MaxSpeed>() };
    std::vector<std::string> sprites{ game()->parser().getVString<parser::Debris, parser::Asteroid, parser::Sprite>() };
    if (spawnChances.size() != masses.size() != masses.size() != speeds.size() && speeds.size() != sprites.size())
        throw CException{ "All arrays of asteroid settings need to have the same size.", INFO };

    ////determine asteroid type
    int rnd{ rand() % std::accumulate(spawnChances.begin(), spawnChances.end(), 0) };
    int itr{ 0 };
    while (rnd > std::accumulate(spawnChances.begin(), spawnChances.begin() + itr + 1, 0))
        ++itr;

    mass(masses[itr]);
    m_maxStartSpeed = speeds[itr];
    std::string gfx{ sprites[itr]};

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
