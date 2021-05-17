//==============================================================================
/// 2021, Benedikt Michael.
//==============================================================================
/// object.h
/// Abstract class for all game objects.
//==============================================================================

#pragma once
#include "maths.h"
#include "exception.h"

#include <utility>
#include <set>


class CGame;
class CComponent;
class CPlayer;
//makeshift for collusion
class CKinetics;
class CSounds;

using colors = olc::Pixel;

enum class objectTypes { BLACKHOLE, STAR, DEBRIS, ENEMY, MISSLE, count };
enum class objectStates { ALIVE, DESTROYED, EATEN, DELETED, count };

//abstract class with all basic object attributes
class CObject
{
public:
	CObject(CGame* const pGame, objectTypes type, v2d pos = { 0.0f, 0.0f }, int mass = 0, colors color = olc::WHITE);

	//for correct initialization and destruction
	virtual ~CObject() = default;

	//function to update the object on each tick
	virtual void update(float deltaTime);
	//has to calculate the collsion boundary of the object in a straight line to another one
	virtual float edge() const = 0;

	void x(float x);
	void y(float y);
	float x() const;
	float y() const;

	void xy(float x, float y);
	void xy(v2d point);
	v2d xy() const;

	void color(colors color);
	colors color() const;

	void mass(int mass);
	int mass() const;

	void state(objectStates state);
	objectStates state() const;

	objectTypes type() const;

	CGame* const game() const;

	//shortcut to the player object
	std::shared_ptr<CPlayer> player() const;

	void addComponent(std::shared_ptr<CComponent> component);

	//checks whether the object (or a point as parameter) is in the viewable map
	bool isInView() const;
	bool isInView(v2d pos) const;

	float fogFactor() const;

	bool isEatable() const;
	void eatable(bool eatable);

	//makeshift for collsision - has to be implemented properly
	CKinetics* m_pKineticsObj;
	CSounds* m_pSoundsObj;

private:
	v2d m_pos;
	colors m_color;
	int m_mass;
	//whether this object could be eaten by a black hole
	bool m_eatable;

	objectTypes m_type;
	objectStates m_state;

	//pointer to the game
	CGame* const m_pGame;
	//set of pointers to all used components
	std::set<std::shared_ptr<CComponent>> m_pComponents;
};