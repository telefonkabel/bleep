//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file asteroid.cpp
/// \brief Asteroid game object.
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


bool CAsteroid::m_firstInit{ true };
std::vector<int> CAsteroid::m_spawnChances{};
std::vector<int> CAsteroid::m_masses{};
std::vector<int> CAsteroid::m_speeds{};
std::vector<std::vector<int>> CAsteroid::m_spawnTimes{};
std::vector<std::string> CAsteroid::m_sprites{};

CAsteroid::CAsteroid(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
    CObject{ pGame, type, pos, mass, color },
    m_maxStartSpeed{},
    m_relevantSpawnChances{}
{
	addComponent(components::kinetics);
	addComponent(components::graphics);

	initAsteroid();
     
	eatable(true);
}

CAsteroid::~CAsteroid()
{
    if (state() == objectStates::EATEN)
        game()->effectEaten();
}

void CAsteroid::initAsteroid()
{
    //load settings
    if (m_firstInit)
    {
        m_firstInit = false;
        m_spawnChances = { game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::SpawnChance>() };
        m_masses = { game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::Mass>() };
        m_speeds = { game()->parser().getVInt<parser::Debris, parser::Asteroid, parser::MaxSpeed>() };
        m_spawnTimes = { game()->parser().getVVInt<parser::Debris, parser::Asteroid, parser::SpawnTime>() };
        m_sprites = { game()->parser().getVString<parser::Debris, parser::Asteroid, parser::Sprite>() };
        size_t asteroidTypes{ m_spawnChances.size() };
        if (asteroidTypes != m_masses.size() && asteroidTypes != m_speeds.size() && asteroidTypes != m_sprites.size()
            && asteroidTypes != m_spawnTimes.size())
            throw CException{ "All arrays of asteroid settings need to have the same size.", INFO };

        //max time interval
        for (std::vector<int>& interval : m_spawnTimes)
            if(interval[1] < 0)
                interval[1] = INT_MAX;
    }

    ////determine asteroid type
    //determine all relevant asteroid types, dependent on their spawn times
    for (int itr{ 0 }; itr < m_spawnChances.size(); ++itr)
    {
        std::chrono::seconds playedTime{ game()->playtime() };
        if (playedTime >= std::chrono::seconds(m_spawnTimes[itr][0]) && playedTime <= std::chrono::seconds(m_spawnTimes[itr][1]))
            m_relevantSpawnChances.push_back(m_spawnChances[itr]);
        else
            m_relevantSpawnChances.emplace_back( 0 );
    }

    std::string gfx{};
    int relevantChances{ std::accumulate(m_relevantSpawnChances.begin(), m_relevantSpawnChances.end(), 0) };
    if (relevantChances)
    {
        int rnd{ rand() % relevantChances };
        int itr{};
        while (rnd >= std::accumulate(m_relevantSpawnChances.begin(), m_relevantSpawnChances.begin() + itr + 1, 0))
            ++itr;

        mass(m_masses[itr]);
        m_maxStartSpeed = m_speeds[itr];
        gfx = m_sprites[itr];

        graphics()->sprite(gfx);

        //start point
        if(xy() == v2d{})
            xy(maths::rndCirclePt(game()->center(), static_cast<float>(game()->radiusMap())));
        //start velocity
        float rndAngle{ maths::rndAngle() };
        int startSpeed{ rand() % m_maxStartSpeed };
        kinetics()->velocity(v2d{ startSpeed * static_cast<float>(cos(rndAngle)), startSpeed * static_cast<float>(sin(rndAngle)) });
    }
    else
        //no real asteroid created;
        state(objectStates::DELETED);
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
