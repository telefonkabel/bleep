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
	//spawn chances that are currently relevant (dependent on its possible spawn times)
	std::vector<int> m_relevantSpawnChances;

	////static arrays of settings for all asteroids for auto-creating each instance
	static bool m_firstInit;
	static std::vector<int> m_spawnChances;
	static std::vector<int> m_masses;
	static std::vector<int> m_speeds;
	static std::vector<std::string> m_sprites;
	static std::vector<std::vector<int>> m_spawnTimes;

	void initAsteroid();
};