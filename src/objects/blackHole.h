//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// blackHole.h
/// Black hole game object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;

class CBHole : public CObject
{
public:
	CBHole(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color);
	~CBHole();

	void update(float deltaTime) override;
	float edge() override;

	int radiusGrav() const;

	void fireHawking(v2d direction, float deltaTime);

private:
	//radius of the black hole
	int m_radius;
	//radius used for lense effect
	int m_radiusGravity;
	//timing for the flickering effect of black holes
	float m_flickerCount;

	//timer for hawking radiation
	float m_hawkingTimer;
	float m_hawkingTimerReload;
	int m_hawkingSpeed;
	int m_hawkingMass;

	void draw() const;
};