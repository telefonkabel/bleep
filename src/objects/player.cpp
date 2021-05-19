//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// player.cpp
/// Player game object.
//==============================================================================

#include "player.h"


CPlayer::CPlayer(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
	CObject(pGame, type, pos, mass, color),
	m_flickerCount{},
	m_radiusGravity{},
	m_radius{}
{}

void CPlayer::update(float deltaTime)
{
	//calculate radii in relation to its mass
	m_radius = mass() / 100000 + 1;
	m_radiusGravity = 5 * m_radius;

	m_flickerCount += deltaTime;
	CObject::update(deltaTime);

	draw();
}

void CPlayer::draw() const
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

float CPlayer::edge() const { return static_cast<float>(m_radius); };
int CPlayer::radiusGrav() const { return m_radiusGravity; };