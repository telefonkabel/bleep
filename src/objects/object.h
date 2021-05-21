//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// object.h
/// Abstract class for all game objects.
//==============================================================================

#pragma once
#include "maths.h"
#include "exception.h"

#include <utility>
#include <set>
#include <variant>


class CGraphics;
class CKinetics;
class CGame;
class CComponent;
class CPlayer;
enum class components;

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
	virtual float edge() = 0;

	//set & get
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

	bool isEatable() const;
	void eatable(bool eatable);

	CGame* const game() const;

	//shortcut to the player object
	std::shared_ptr<CPlayer> player() const;

	//hook a component onto the object
	void addComponent(components component);
	//handles to the components
	std::unique_ptr<CKinetics>& kinetics();
	std::unique_ptr<CGraphics>& graphics();

	//checks whether the object (or a point as parameter) is in the viewable map
	bool isInView() const;
	bool isInView(v2d pos) const;

	//the density of the fog of war
	float fogFactor() const;

private:
	v2d m_pos;
	colors m_color;
	int m_mass;
	//determines whether the object could be sucked into a black hole
	bool m_eatable;

	//the type and state of the object
	objectTypes m_type;
	objectStates m_state;

	//pointer to the game object
	CGame* const m_pGame;

	//handling of components
	std::unique_ptr<CKinetics> m_pKinetics;
	std::unique_ptr<CGraphics> m_pGraphics;
	std::set<components> m_components;
};