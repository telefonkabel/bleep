//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// hawking.h
/// Hawking missile game object.
//==============================================================================

#pragma once
#include "object.h"


//Circle-octants named after their position in each coordinate system quadrant with values used as uint_8 (used in olc)
enum class octant { Q11 = 2, Q12 = 1, Q21 = 128, Q22 = 64, Q31 = 32, Q32 = 16, Q41 = 8, Q42 = 4 };

class CGame;

class CHawking : public CObject
{
public:
	CHawking(CGame* const pGame, objectTypes type, int mass = 0, v2d pos = { 0.0f, 0.0f }, colors color = olc::WHITE);
	~CHawking();

	void update(float deltaTime) override;
	float edge() override;


private:
	static bool m_firstInit;
	static int m_radius;

	//for drawing direction related sprites - 8 possibilities
	uint8_t m_direction;

	void calcDirection();
};