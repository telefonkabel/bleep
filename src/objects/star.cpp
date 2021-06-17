//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file star.cpp
/// \brief Background star object.
//==============================================================================

#include "star.h"
#include "blackHole.h"
#include "game.h"
#include "components/graphics.h"
#include "components/kinetics.h"


CStar::CStar(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
	CObject{ pGame, type, pos, mass, color },
	m_flickerEffect{},
	m_flickerEffectTime{ 0.3f },
	m_flickerTimer{},
	m_flickerTimerReload{ 0.01f }
{
	addComponent(components::kinetics);
}

void CStar::update(float deltaTime)
{
	//recycle the star object
	if ((game()->center() - xy()).mag() > game()->radiusView())
		xy(maths::pointReflection(game()->center(), xy(), static_cast<float>(game()->radiusView())));

	//random flickering
	m_flickerEffect = m_flickerEffect <= 0.0f ? 0.0f : m_flickerEffect - deltaTime;
	m_flickerTimer -= deltaTime;
	if (m_flickerTimer <= 0.0f)
	{
		m_flickerTimer = m_flickerTimerReload;
		if (rand() % RAND_MAX == 0)
			m_flickerEffect = m_flickerEffectTime;
	}

	CObject::update(deltaTime);
	draw();
}

void CStar::draw() const
{
	v2d offset{};
	//lens effect near black hole (for one black hole for now)
	std::shared_ptr<CBHole> player{ game()->player() };
	if (player)
	{
		int lensRadius{ player->radiusGrav() };
		v2d v2Player{ xy() - player->xy() };
		float dist2Player{ v2Player.mag() };
		float lensFactor{ 1 - std::min(dist2Player / lensRadius, 1.0f) };

		offset = v2Player.norm() * static_cast<float>(player->edge()) * lensFactor;
	}
	v2d positionWithLens{ xy() + offset };

	//draw
	if (!isInView(positionWithLens))
		return;

	game()->Draw(positionWithLens, color() * fogFactor());

	//flickering
	if (m_flickerEffect != 0.0f)
	{
		float fog{ fogFactor() };
		game()->Draw(positionWithLens, color() * std::min(fog, 1.0f));
		game()->Draw(positionWithLens + v2d{ 0, 1 }, color()* std::min(fog, 1.0f) / 2);
		game()->Draw(positionWithLens + v2d{ 0, -1 }, color() * std::min(fog, 1.0f) / 2);
		game()->Draw(positionWithLens + v2d{ 1, 0 }, color() * std::min(fog, 1.0f) / 2);
		game()->Draw(positionWithLens + v2d{ -1, 0 }, color() * std::min(fog, 1.0f) / 2);
		game()->Draw(positionWithLens + v2d{ 0, 2 }, color() * std::min(fog, 1.0f) / 4);
		game()->Draw(positionWithLens + v2d{ 0, -2 }, color() * std::min(fog, 1.0f) / 4);
		game()->Draw(positionWithLens + v2d{ 2, 0 }, color() * std::min(fog, 1.0f) / 4);
		game()->Draw(positionWithLens + v2d{ -2, 0 }, color() * std::min(fog, 1.0f) / 4);
	}
}

float CStar::edge() { return 0.0f; };
