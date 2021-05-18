//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// kinetics.h
/// Component class which handles all object kinetics.
//==============================================================================

#pragma once
#include "component.h"
#include "game.h"

#include <deque>


class CKinetics : public CComponent
{
public:
	CKinetics(CObject* pObject);

	void update(float deltaTime) override;

	//set & get
	void speedFactor(float fac);
	float speedFactor() const;

	v2d velocity() const;
	void velocity(v2d velocity);

	v2d acceleration() const;
	void acceleration(v2d acceleration);

	//returns the plain speed
	float speed() const;

private:
	v2d m_velocity;
	v2d m_acceleration;

	//constant speed factor (f.e. to simulate depth)
	float m_speedFactor;

	//brings inertia and other velocities/accelerations together
	void calcVelocity(float deltaTime);

	//makeshift function which handels all kinds of collision -> should be reworked
	void collision();

	//helper function to apply gravity and game velocity
	v2d applyVelocities(float deltaTime);

	//pointer to all game objects
	const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>* m_pGameObjects;
};