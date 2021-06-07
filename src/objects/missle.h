//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// missle.h
/// Missle game object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;

class CMissle : public CObject
{
public:
	CMissle(CGame* const pGame, objectTypes type, int mass = 0, v2d pos = { 0.0f, 0.0f }, colors color = olc::WHITE);
	~CMissle();

	void update(float deltaTime) override;
	float edge() override;

private:
};