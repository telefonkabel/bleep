//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// kinetics.cpp
/// Component class which handles all object kinetics.
//==============================================================================

#include "kinetics.h"
#include "player.h"

#include <numeric>

//makeshift for collsision - has to be implemented properly
#include "sounds.h"


CKinetics::CKinetics(CObject* pObject) :
	CComponent{ pObject },
	m_velocity{},
	m_speedFactor{ 1.0f },
	m_acceleration{},
	m_pGameObjects{ nullptr }
{
	m_pGameObjects = &object()->game()->gameObjects();
}

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
	////calculate gravity
	v2d v2Player{ object()->player()->xy() - object()->xy() };
	v2d gravity{};
	if (object()->mass() != 0)
	{
		//calculate gravity magnitude
		gravity = v2Player.norm() * (object()->player()->mass() / v2Player.mag2());
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
	////check vor collision
	//collision with outer boundaries
	if((object()->xy() - object()->game()->center()).mag() > object()->game()->radiusMap())
		object()->state(objectStates::DELETED);
	//collsision with objects
	for (int listID{ 0 }; listID < static_cast<int>(objectTypes::count); ++listID)
	{
		//collision with black holes
		if (listID == static_cast<int>(objectTypes::BLACKHOLE))
		{
			if (object()->isEatable())
			{
				for (auto& obj : m_pGameObjects->at(listID))
					if ((object()->xy() - obj->xy()).mag() <= /*object()->edge() +*/ obj->edge())
						object()->state(objectStates::EATEN);
			}
		}
		//collision with other objects
		else if (listID == static_cast<int>(objectTypes::DEBRIS) || listID == static_cast<int>(objectTypes::ENEMY))
		{
			for (auto& obj : m_pGameObjects->at(listID))
			{
				float distance{ (object()->xy() - obj->xy()).mag() };
				float touchDistance{ object()->edge() + obj->edge() };
				//"> 0" excludes comparison with itself
				if (distance > 0.1f && distance < touchDistance)
				{
					//set distance to touchDistance after first contact (or initialization) to stop unwanted "repetition" (there should be a better solution)
					object()->xy(object()->xy() + (object()->xy() - obj->xy()).norm() * ((touchDistance - distance) / 2));
					obj->xy(obj->xy() - (object()->xy() - obj->xy()).norm() * ((touchDistance - distance) / 2));

					//apply collision velocities
					v2d objVel{ obj->m_pKineticsObj->velocity() };
					v2d velNeu{ maths::elasticDiskImpact(m_velocity, static_cast<float>(object()->mass()), objVel, static_cast<float>(obj->mass())) };
					v2d objVelNeu{ maths::elasticDiskImpact(objVel, static_cast<float>(obj->mass()), m_velocity, static_cast<float>(object()->mass())) };
					velocity(velNeu);
					obj->m_pKineticsObj->velocity(objVelNeu);

					//play sound if in view
					if(object()->isInView())
						object()->game()->sound()->playSound(sounds::CRASH0, false);
				}
			}
		}
	}
}