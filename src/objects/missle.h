//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// missle.h
/// Missle game object.
//==============================================================================

#pragma once
#include "object.h"


//Circle-octants named after their position in each coordinate system quadrant with values used as uint_8 (used in olc)
enum class octant { Q11 = 2, Q12 = 1, Q21 = 128, Q22 = 64, Q31 = 32, Q32 = 16, Q41 = 8, Q42 = 4 };

class CGame;

class CMissle : public CObject
{
public:
	CMissle(CGame* const pGame, objectTypes type, int mass = 0, v2d pos = { 0.0f, 0.0f }, colors color = olc::WHITE, int radius = 8);
	~CMissle();

	void update(float deltaTime) override;
	float edge() override;


private:
	int m_radius;

	//for drawing direction related sprites - 8 possibilities
	uint8_t m_direction;

	void calcDirection();
};