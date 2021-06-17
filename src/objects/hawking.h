//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file hawking.h
/// \brief Hawking missile game object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;
/// \brief Enum for octants.
/// This is used for a more readable way to address the circle octants used by olc.
/// The octants are named after their position in a Cartesian coordinate system.
enum class octant { Q11 = 2, Q12 = 1, Q21 = 128, Q22 = 64, Q31 = 32, Q32 = 16, Q41 = 8, Q42 = 4 };

/// \brief Hawking missile class.
/// This is a concrete object class and overwrites the edge and update fct.
class CHawking : public CObject
{
public:
	/// \brief Constructor of hawking class.
	/// @param pGame Handle to the central game object.
	/// @param type Its object type.
	/// @param pos Its start position.
	/// @param mass Its start mass.
	/// @param color Its color.
	CHawking(CGame* const pGame, objectTypes type, v2d pos = { 0.0f, 0.0f }, int mass = 0, colors color = olc::WHITE);
	/// \brief Destructor of hawking class.
	/// This plays the destruction sound of this obj.
	~CHawking();

	/// \brief Update fct.
	/// This is called by the game update fct and calls the update fct of its base class.
	/// @param deltaTime Elapsed time since last frame.
	void update(float deltaTime) override;
	/// \brief Calculates the collision boundary of the obj.
	/// This overwrites the edge fct of its base class.
	/// @returns 1, cause that edge seems to fit best for this missile object.
	float edge() override;


private:
	/// \brief Flag to check if this is the first hawking instance.
	/// Needed to parse in the settings.
	static bool m_firstInit;
	/// \brief Radius of the circle where the visible octant is cut out.
    /// Parsed out of settings.
	static int m_radius;

	/// \brief This stores the current octant.
	/// This is used to show that octant which resembles the current movement direction best.
	uint8_t m_direction;

	/// \brief Calculates the current direction and sets CHawking::m_direction.
	void calcDirection();
};