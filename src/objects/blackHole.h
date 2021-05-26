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

	void update(float deltaTime) override;
	float edge() override;

	int radiusGrav() const;

private:
	//radius of the black hole
	int m_radius;
	//radius used for lense effect
	int m_radiusGravity;
	//timing for the flickering effect of black holes
	float m_flickerCount;

	void draw() const;
};