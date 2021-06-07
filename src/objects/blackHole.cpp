//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// blackHole.cpp
/// Black hole game object.
//==============================================================================

#include "blackHole.h"
#include "game.h"
#include "components/graphics.h"
#include "components/kinetics.h"

#include "asteroid.h"


CBHole::CBHole(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
	CObject(pGame, type, pos, mass, color),
	m_flickerCount{},
	m_radiusGravity{},
	m_radius{},
	m_hawkingTimer{},
	m_hawkingTimerReload{ pGame->parser().getFloat<parser::Missle, parser::Hawking, parser::SpawnReload>() }
{}

void CBHole::update(float deltaTime)
{
	//calculate radii in relation to its mass
	m_radius = mass() / 100000 + 1;
	m_radiusGravity = 5 * m_radius;

	m_flickerCount += deltaTime;
	CObject::update(deltaTime);

	draw();
}

void CBHole::draw() const
{
	float flickerFactor(std::sin(m_flickerCount * 2) + 1.5f);
	game()->DrawCircle(game()->center(), m_radius + 3, color() * flickerFactor / 8.0f);
	game()->DrawCircle(game()->center(), m_radius + 2, color() * flickerFactor / 6.0f);
	game()->DrawCircle(game()->center(), m_radius + 1, color() * flickerFactor / 4.0f);
	game()->DrawCircle(game()->center(), m_radius + 0, color() * flickerFactor / 2.0f);
	game()->DrawCircle(game()->center(), m_radius - 1, color() * flickerFactor);
	game()->DrawCircle(game()->center(), m_radius - 2, color() * flickerFactor / 4.0f);
	game()->FillCircle(game()->center(), m_radius - 3, olc::BLACK);
}

float CBHole::edge() { return static_cast<float>(m_radius); };
int CBHole::radiusGrav() const { return m_radiusGravity; };

void CBHole::fireHawking(v2d direction, float deltaTime)
{
	m_hawkingTimer -= deltaTime;
	if (m_hawkingTimer <= 0.0f)
	{
		m_hawkingTimer = m_hawkingTimerReload;
		std::shared_ptr<CAsteroid> test{ std::make_shared<CAsteroid>(game(), objectTypes::DEBRIS,  1000, game()->getCursor()) };
		test->kinetics()->velocity(game()->getCursor() - xy());
		game()->addObject(std::move(test));
	}
}