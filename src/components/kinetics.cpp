//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file kinetics.cpp
/// \brief Component class which handles all object kinetics.
//==============================================================================

#include "kinetics.h"
#include "game.h"
#include "sound.h"
#include "objects/blackHole.h"

#include <numeric>


CKinetics::CKinetics(CObject* pObject) :
	CComponent{ pObject },
	m_velocity{},
	m_speedFactor{ 1.0f },
	m_acceleration{},
	m_pGameObjects{ object()->game()->gameObjects() }
{}

void CKinetics::update(float deltaTime)
{
	if(object()->type() != objectTypes::STAR)
		collision();
	calcVelocity(deltaTime);
}

float CKinetics::speed() const { return  velocity().mag(); };

void CKinetics::speedFactor(float fac) { m_speedFactor = fac; };
float CKinetics::speedFactor() const { return m_speedFactor; };

v2d CKinetics::velocity() const { return m_velocity; };
void CKinetics::velocity(v2d velocity) { m_velocity = velocity; };

v2d CKinetics::acceleration() const { return m_acceleration; };
void CKinetics::acceleration(v2d acceleration) { m_acceleration = acceleration; };

v2d CKinetics::applyVelocities(float deltaTime)
{
	////calculate gravity (for one black hole for now)
	v2d gravity{};
	if (object()->mass() != 0 && object()->type() != objectTypes::MISSILE)
	{
		//for now for only one black hole
		std::shared_ptr<CBHole> player{ object()->game()->player() };
		if (player)
		{
			v2d v2Player{ player->xy() - object()->xy() };
			//calculate gravity magnitude
			gravity = v2Player.norm() * (player->mass() / v2Player.mag2());
		}
	}
	//apply gravity
	m_velocity += (m_acceleration + gravity) * deltaTime;

	//add game velocity
	return m_velocity + object()->game()->velocity();
}

void CKinetics::calcVelocity(float deltaTime)
{
	object()->xy(object()->xy() + (applyVelocities(deltaTime) * speedFactor() * deltaTime));
}

void CKinetics::collision()
{
	//missiles are processed through other objects
	if (object()->type() == objectTypes::MISSILE)
		return;

	////check for collision
	//collision with outer boundaries
	if((object()->xy() - object()->game()->center()).mag() > object()->game()->radiusMap())
		object()->state(objectStates::DELETED);
	//collision with objects
	for (int listID{ 0 }; listID < static_cast<int>(objectTypes::count); ++listID)
	{
		switch (listID)
		{
			//no collision with stars
			case static_cast<int>(objectTypes::STAR) :
				break;
			//collision with black holes
			case static_cast<int>(objectTypes::BLACKHOLE):
				if (object()->isEatable())
				{
					for (auto& obj : m_pGameObjects.at(listID))
					{
						if (isCollision(obj))
						{
							object()->state(objectStates::EATEN);
							obj->mass(obj->mass() + object()->mass());
						}
					}
				}
				break;
			//collision with massive objects
			case static_cast<int>(objectTypes::ENEMY) :
			case static_cast<int>(objectTypes::DEBRIS) :
				for (auto& obj : m_pGameObjects.at(listID))
				{
					if (isCollision(obj))
					{
						//set distance to touchDistance after first contact (or initialization) to stop unwanted "repetition"
						v2d offset{ (object()->xy() - obj->xy()).norm() * (collisionOverlap(obj) / 1.9f) };
						object()->xy(object()->xy() + offset);
						obj->xy(obj->xy() - offset);

						//apply collision velocities
						v2d objVel{ obj->kinetics()->velocity() };
						v2d velNeu{ maths::elasticDiskImpact(m_velocity, static_cast<float>(object()->mass()), objVel, static_cast<float>(obj->mass())) };
						v2d objVelNeu{ maths::elasticDiskImpact(objVel, static_cast<float>(obj->mass()), m_velocity, static_cast<float>(object()->mass())) };
						velocity(velNeu);
						obj->kinetics()->velocity(objVelNeu);

						if (object()->isInView())
							object()->game()->sound().playSound(sounds::CRASH0, false);
					}
				}
				break;
			//collision with missile object
			case static_cast<int>(objectTypes::MISSILE) :
				for (auto& obj : m_pGameObjects.at(listID))
				{
					if (isCollision(obj))
					{
						//apply collision velocities//apply collision velocities
						v2d objVel{ obj->kinetics()->velocity() };
						v2d velNeu{ maths::inelasticDiskImpact(m_velocity, static_cast<float>(object()->mass()), objVel, static_cast<float>(obj->mass())) };
						velocity(velNeu);

						obj->state(objectStates::DESTROYED);
					}
				}
				break;
			default:
				throw CException{ listID + " is no handled object type ID - should not happen.", INFO };
				break;
		}
	}
}

bool CKinetics::isCollision(const std::shared_ptr<CObject>& obj) const
{
	return (collisionOverlap(obj) > 0 && object() != obj.get());
}

float CKinetics::collisionOverlap(const std::shared_ptr<CObject>& obj) const
{
	//difference of should-be touch distance with actual distance (positive if touchDistance > distance)
	return (object()->edge() + obj->edge()) - (object()->xy() - obj->xy()).mag();
}