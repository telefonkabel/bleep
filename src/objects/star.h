//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// star.h
/// Background star object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;

class CStar : public CObject
{
public:
	CStar(CGame* const pGame, objectTypes type, v2d pos = { 0.0f, 0.0f }, int mass = 0, colors color = olc::WHITE);

	void update(float deltaTime) override;
	float edge() override;

private:
	//to generate a random flickering unrelated to the framerate
	float m_flickerEffect;
	float m_flickerEffectTime;
	float m_flickerTimer;
	float m_flickerTimerReload;

	void draw() const;
};