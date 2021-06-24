//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file asteroid.h
/// \brief Asteroid game object.
//==============================================================================

#pragma once
#include "object.h"


namespace bleep
{

	class CGame;

	/// \brief Asteroid class.
	/// This is a concrete object class and overwrites the edge and update fct.
	class CAsteroid : public CObject
	{
	public:
		/// \brief Constructor of asteroid class.
		/// @param pGame Handle to the central game object.
		/// @param type Its object type.
		/// @param pos Its start position.
		/// @param mass Its start mass.
		/// @param color Its color.
		CAsteroid(CGame* const pGame, objectTypes type, v2d pos = { 0.0f, 0.0f }, int mass = 0, colors color = olc::WHITE);
		/// \brief Destructor of asteroid class.
		/// This plays the eaten effect if applicable.
		~CAsteroid();

		/// \brief Update fct.
		/// This is called by the game update fct and calls the update fct of its base class.
		/// @param deltaTime Elapsed time since last frame.
		void update(float deltaTime) override;
		/// \brief Calculates the collision boundary of the obj.
		/// This overwrites the edge fct of its base class.
		/// @returns The average boundary - asteroid sprites should therefore have rough disc/square shape.
		float edge() override;

	private:
		/// \brief Maximal start speed.
		/// Parsed out of settings.
		int m_maxStartSpeed;
		/// \brief Helper array which holds the relevant asteroid spawn chances at the moment of initialization.
		std::vector<int> m_relevantSpawnChances;

		//===== static arrays of settings for all asteroids for auto-creating each instance =====
		/// \brief Flag to check if this is the first asteroid instance.
		static bool m_firstInit;
		/// \brief Array of all asteroid type spawn chances.
		/// Parsed out of settings.
		/// \note All asteroid type settings arrays need to have the same size (same amount of asteroid types) in the JSON file.
		static std::vector<int> m_spawnChances;
		/// \brief Array of all asteroid type masses.
		/// Parsed out of settings.
		/// \note All asteroid type settings arrays need to have the same size (same amount of asteroid types) in the JSON file.
		static std::vector<int> m_masses;
		/// \brief Array of all asteroid type maximal start speeds.
		/// Parsed out of settings.
		/// \note All asteroid type settings arrays need to have the same size (same amount of asteroid types) in the JSON file.
		static std::vector<int> m_speeds;
		/// \brief Array of all asteroid type sprites.
		/// Parsed out of settings.
		/// \note All asteroid type settings arrays need to have the same size (same amount of asteroid types) in the JSON file.
		static std::vector<std::string> m_sprites;
		/// \brief Array of all asteroid type spawn time intervals.
		/// Parsed out of settings.
		/// \note All asteroid type settings arrays need to have the same size (same amount of asteroid types) in the JSON file.
		static std::vector<std::vector<int>> m_spawnTimes;

		/// \brief Starts init sequence of an asteroid.
		void initAsteroid();
	};

} //end of namespace bleep