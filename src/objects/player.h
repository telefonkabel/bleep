//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// player.h
/// Player game object.
//==============================================================================

#pragma once
#include "game.h"


class CPlayer : public CObject
{
public:
	CPlayer(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color);

	void update(float deltaTime) override;
	float edge() const override;

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