//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file blackHole.h
/// \brief Black hole game object.
//==============================================================================

#pragma once
#include "object.h"


namespace bleep
{

	class CGame;

	/// \brief Black hole class.
	/// This is a concrete object class and overwrites the edge and update fct.
	class CBHole : public CObject
	{
	public:
		/// \brief Constructor of star class.
		/// @param pGame Handle to the central game object.
		/// @param type Its object type.
		/// @param pos Its start position.
		/// @param mass Its start mass.
		/// @param color Its color.
		CBHole(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color);

		/// \brief Update fct.
		/// This is called by the game update fct and calls the update fct of its base class.
		/// @param deltaTime Elapsed time since last frame.
		void update(float deltaTime) override;
		/// \brief Calculates the collision boundary of the obj.
		/// This overwrites the edge fct of its base class.
		/// @returns The radius of the black hole.
		float edge() override { return static_cast<float>(m_radius); };

		/// \brief The radius where the gravity gets applied.
		/// Currently the design applies this only to the gravity lens distorting the background stars - all other objects are always subject to the gravity.
		/// @returns The gravity radius: CBHole::m_radiusGravity.
		int radiusGrav() const { return m_radiusGravity; };

		/// \brief Manages the release of hawking radiation.
		/// This gets called by the game loop.
		/// @param direction The shooting direction.
		/// @param deltaTime Elapsed time since last frame.
		void fireHawking(v2d direction, float deltaTime);

	private:
		/// \brief Radius of the black hole.
		int m_radius;
		/// \brief Radius where its gravity applies - currently set to 5 * radius.
		int m_radiusGravity;
		/// \brief A deltaTime and sine related value for the dimming effect of black holes.
		float m_flickerCount;

		/// \brief Delay between firing.
		/// During firing attempts this gets steadily counted down till it reaches 0, then it gets set back to the reload time.
		float m_hawkingTimer;
		/// \brief Delay between firing.
		/// Parsed out of settings.
		float m_hawkingTimerReload;
		/// \brief Missile speed.
		/// Parsed out of settings.
		int m_hawkingSpeed;
		/// \brief Missile mass.
		/// Parsed out of settings.
		int m_hawkingMass;

		/// \brief Draws the star.
		/// This takes the dimming effect into account.
		void draw() const;
	};

} //end of namespace bleep