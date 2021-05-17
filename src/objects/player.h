//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file player.h
/// \brief Player game object.
//==============================================================================

#pragma once
#include "game.h"


class CGame;
class CSounds;

class CPlayer : public CObject
{
public:
	CPlayer(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color);

	void update(float deltaTime) override;
	float edge() const override;

	int radiusEvent() const;
	int radiusGrav() const;

private:
	int m_radius;
	int m_radiusEvent;
	int m_radiusGravity;
	float m_flickerCount;

	std::shared_ptr<CSounds> m_pSounds;

	void draw() const;
};