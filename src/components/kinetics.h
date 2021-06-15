//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file kinetics.h
/// \brief Component class which handles all object kinetics.
//==============================================================================

#pragma once
#include "component.h"
#include "objects/object.h"


/// \brief Kinetics class.
/// This is concrete component class and overwrites the update fct.
class CKinetics : public CComponent
{
public:
	/// \brief Constructor of kinetics class.
	/// @param pObject Handle to the corresponding object.
	CKinetics(CObject* pObject);

	/// \brief Update fct.
	/// This is called along the central update fct and calculates all of its object's motion related issues.
    /// @param deltaTime Elapsed time since last frame.
	void update(float deltaTime) override;

	//===== getter & setter =====
	/// \brief Getter for the current speed factor.
	/// @returns float of CKinetics::m_speedFactor.
	float speedFactor() const;
	/// \brief Setter for the speed factor.
	/// This gets multiplied with the calculated velocity and could be used to distinguish similar objects - like to simulate depth.
	/// @returns float of CKinetics::m_speedFactor.
	void speedFactor(float fac);
	/// \brief Getter for the current velocity.
	/// @returns 2D vector of CKinetics::m_velocity.
	v2d velocity() const;
	/// \brief Setter for velocity.
	/// @param velocity New velocity as 2D vector.
	void velocity(v2d velocity);
	/// \brief Getter for the current acceleration setting.
	/// @returns 2D vector of CKinetics::m_acceleration.
	v2d acceleration() const;
	/// \brief Setter for acceleration setting.
	/// @param acceleration New acceleration as 2D vector.
	void acceleration(v2d acceleration);
	/// \brief Getter for the current plain speed.
	/// @returns Calculated speed as float.
	float speed() const;

private:
	/// \brief Object velocity.
	/// Stores the current velocity.
	v2d m_velocity;
	/// \brief Object velocity.
	/// Stores a possible object specific acceleration setting.
	v2d m_acceleration;

	/// \brief Speed factor of the obj.
	/// Stores a factor which gets applied after the overall velocity is calculated. Could be useful at some occasions.
	float m_speedFactor;

	/// \brief Calculates velocity.
	/// Brings inertia and other velocities/accelerations together.
    /// @param deltaTime Elapsed time since last frame.
	void calcVelocity(float deltaTime);

	/// \brief Handles all kinds of collision.
	void collision();
	/// \brief Determines if a collision happened.
	/// Helper fct to determine if collision happened with a specific object.
    /// @param obj The object which could have collided with this object.
	bool isCollision(const std::shared_ptr<CObject>& obj) const;
	//helper fct which calculates possible collision overlap

	/// \brief Calculates the overlapping of a collision.
	/// Since deltaTime is not infinitesimal small, there will be always an overlap when something collides.
	/// @param obj The object which has collided with this object.
	float collisionOverlap(const std::shared_ptr<CObject>& obj) const;

	/// \brief Apply all kinds of velocities.
	/// This applies global vectors like gravity or player movement.
	/// @param deltaTime Elapsed time since last frame.
	v2d applyVelocities(float deltaTime);

	/// \brief Handle to all game objects.
	/// This Ref is save, cause the game array of all global objects has static lifetime.
	const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>& m_pGameObjects;
};