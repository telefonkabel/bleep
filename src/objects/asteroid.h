//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// asteroid.h
/// Asteroid game object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;

class CAsteroid : public CObject
{
public:
	CAsteroid(CGame* const pGame, objectTypes type, int mass = 0, v2d pos = { 0.0f, 0.0f }, colors color = olc::WHITE);
	~CAsteroid();

	void update(float deltaTime) override;
	float edge() override;

private:
	int m_maxStartSpeed;

	void initAsteroid();
};