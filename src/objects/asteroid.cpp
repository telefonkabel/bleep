#include "asteroid.h"
#include "game.h"
#include "player.h"
#include "components/kinetics.h"
#include "components/graphics.h"
#include "components/sounds.h"


CAsteroid::CAsteroid(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color) :
    CObject(pGame, type, pos, mass, color),
    m_pKinetics(std::make_shared<CKinetics>(this)),
    m_pGraphics(std::make_shared<CGraphics>(this)),
    m_pSounds(std::make_shared<CSounds>(this)),
    m_maxStartSpeed(200)
{
	addComponent(m_pKinetics);
	addComponent(m_pGraphics);
	addComponent(m_pSounds);

	initAsteroid();
     
	eatable(true);

    //makeshift for collusion - needs to be implemented properly
    m_pKineticsObj = m_pKinetics.get();
}

CAsteroid::~CAsteroid()
{
    if (state() == objectStates::EATEN)
    {
        player()->mass(player()->mass() + mass());
        m_pSounds->playSound(sounds::JET, false);
    }
}

void CAsteroid::initAsteroid()
{
    //asteroid type
    int rdn(rand() % 100);
    graphics gfx;
    if (rdn < 35)
    {
        mass(10000);
        gfx = graphics::ASTR_SMALL1;
    }
    else if (rdn < 70)
    {
        mass(10000);
        gfx = graphics::ASTR_SMALL2;
    }
    else if (rdn < 95)
    {
        mass(20000);
        gfx = graphics::ASTR_MEDIUM;
    }
    else
    {
        mass(40000);
        gfx = graphics::ASTR_BIG;
    }
    m_pGraphics->sprite(objectTypes::DEBRIS, gfx);

    //start point
    xy(maths::rndCirclePt(game()->center(), static_cast<float>(game()->radiusMap())));
    //start velocity
    float rndAngle(maths::rndAngle());
    int startSpeed(rand() % m_maxStartSpeed);
    m_pKinetics->velocity(v2d({ startSpeed * static_cast<float>(cos(rndAngle)), startSpeed * static_cast<float>(sin(rndAngle)) }));
}

void CAsteroid::update(float deltaTime)
{
	CObject::update(deltaTime);
}

float CAsteroid::edge() const
{
    return static_cast<float>((m_pGraphics->width() + m_pGraphics->height())) / 4.0f;
}
