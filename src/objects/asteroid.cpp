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


bool CAsteroid::m_firstInit{ true };
std::vector<int> CAsteroid::m_spawnChances{};
std::vector<int> CAsteroid::m_masses{};
std::vector<int> CAsteroid::m_speeds{};
std::vector<std::vector<int>> CAsteroid::m_spawnTimes{};
std::vector<std::string> CAsteroid::m_sprites{};

CAsteroid::CAsteroid(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color) :
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
    {
        blackHoles().front()->mass(blackHoles().front()->mass() + mass());
        game()->sound().playSound(sounds::JET, false);
    }
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
    }

    ////determine asteroid type
    //determine all relevant asteroid types, dependent on their spawn times
    int itrChances{};
    for (auto& chance : m_spawnChances)
    {
        std::chrono::seconds playedTime{ game()->playtime() };
        if (playedTime >= std::chrono::seconds(m_spawnTimes[itrChances][0]) && playedTime <= std::chrono::seconds(m_spawnTimes[itrChances][1]))
            m_relevantSpawnChances.push_back(chance);
        else
            m_relevantSpawnChances.emplace_back( 0 );
        ++itrChances;
    }

    std::string gfx{};
    int relevantChances{ std::accumulate(m_relevantSpawnChances.begin(), m_relevantSpawnChances.end(), 0) };
    if (relevantChances)
    {
        int rnd{ rand() % std::accumulate(m_relevantSpawnChances.begin(), m_relevantSpawnChances.end(), 0) };
        int itr{};
        while (rnd > std::accumulate(m_relevantSpawnChances.begin(), m_relevantSpawnChances.begin() + itr + 1, 0))
            ++itr;

        mass(m_masses[itr]);
        m_maxStartSpeed = m_speeds[itr];
        gfx = m_sprites[itr];

        graphics()->sprite(objectTypes::DEBRIS, gfx);

        //start point
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
