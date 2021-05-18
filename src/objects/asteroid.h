//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// asteroid.h
/// Asteroid game object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;
class CKinetics;
class CGraphics;
class CSounds;

class CAsteroid : public CObject
{
public:
	CAsteroid(CGame* const pGame, objectTypes type, int mass = 0, v2d pos = { 0.0f, 0.0f }, colors color = olc::WHITE);
	~CAsteroid();

	void update(float deltaTime) override;
	float edge() const override;

private:
	std::shared_ptr<CKinetics> m_pKinetics;
	std::shared_ptr<CGraphics> m_pGraphics;
	std::shared_ptr<CSounds> m_pSounds;

	int m_maxStartSpeed;

	void initAsteroid();
};