//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file object.h
/// \brief Abstract class for all game objects.
//==============================================================================

#pragma once

#include "jsParser.h"

#include "helper/maths.h"
#include "helper/exception.h"

#include <set>


namespace bleep
{

	class CGame;
	class CGraphics;
	class CKinetics;
	enum class components;

	/// \brief Typedef for convenience.
	using colors = olc::Pixel;

	/// \brief Enum for object types.
	/// This is used for a more readable way to distinguish objects.
	/// \note For proper counting this enum mustn't have initializers.
	enum class objectTypes { BLACKHOLE, STAR, DEBRIS, ENEMY, MISSILE, count };
	/// \brief Enum for object states.
	/// This is used for a more readable way to handle object states.
	/// \note For proper counting this enum mustn't have initializers.
	enum class objectStates { ALIVE, DESTROYED, EATEN, DELETED, count };

	/// \brief Basic object class.
	/// This is an abstract object class with pure virtual edge fct.
	class CObject
	{
	public:
		/// \brief Constructor of object class.
		/// @param pGame Handle to the central game object.
		/// @param type Its object type.
		/// @param pos Its start position.
		/// @param mass Its start mass.
		/// @param color Its color - if dyeable.
		CObject(CGame* const pGame, objectTypes type, v2d pos = { 0.0f, 0.0f }, int mass = 0, colors color = olc::WHITE);

		/// \brief Virtual destructor of object class.
		/// Improves code safety for not being able to be called directly.
		virtual ~CObject() = default;

		/// \brief Virtual update fct.
		/// This is called by the update fct of its child and calls f.e. the update fct of its components.
		/// @param deltaTime Elapsed time since last frame.
		virtual void update(float deltaTime);
		/// \brief Pure virtual fct to get the collision boundary of the obj.
		/// Each obj has to overwrite this to handle its own distinct boundaries.
		/// \note For now there is only a fct needed which returns the average boundary, cause the implemented objects can be approximated as a disc. 
		/// @returns The implementations have to return their distance to the boundary.
		virtual float edge() = 0;

		//===== getter & setter =====

		/// \brief Getter for the current x coordinate.
		/// @returns X component of its 2D position.
		float x() const { return m_pos.x; };
		/// \brief Getter for the current y coordinate.
		/// @returns Y component of its 2D position.
		float y() const { return m_pos.y; };
		/// \brief Setter for x coordinate.
		/// @param x New x component for its 2D position.
		void x(float x) { m_pos.x = x; };
		/// \brief Setter for y coordinate.
		/// @param y New y component for its 2D position.
		void y(float y) { m_pos.y = y; };
		/// \brief Getter for its current coordinates.
		/// @returns 2D vector of its position.
		v2d xy() const { return m_pos; };
		/// \brief Setter for its coordinates.
		/// @param x New x component for its 2D position.
		/// @param y New y component for its 2D position.
		void xy(float x, float y) { m_pos = { x, y }; };
		/// \brief Setter for its coordinates.
		/// @param point 2D vector to its new position.
		void xy(v2d point) { m_pos = point; };
		/// \brief Getter for its current color.
		/// @returns Color: CObject::m_color.
		colors color() const { return m_color; };
		/// \brief Setter for its color.
		/// @param color Its new color.
		void color(colors color) { m_color = color; };
		/// \brief Getter for its current mass.
		/// @returns Int of CObject::m_mass.
		int mass() const { return m_mass; };
		/// \brief Setter for its mass.
		/// @param mass Its new mass.
		void mass(int mass) { m_mass = mass; };
		/// \brief Getter for its state.
		/// @returns Sate: CObject::m_state.
		objectStates state() const { return m_state; };
		/// \brief Setter for its state.
		/// @param state Its new state.
		void state(objectStates state) { m_state = state; };
		/// \brief Getter for its type.
		/// @returns Type: CObject::m_type.
		objectTypes type() const { return m_type; };
		/// \brief Getter for its eatable flag.
		/// @returns bool: CObject::m_eatable.
		bool isEatable() const { return m_eatable; };
		/// \brief Setter for its eatable flag.
		/// @param eatable Flag for eatability.
		void eatable(bool eatable) { m_eatable = eatable; };
		/// \brief Handle to the central game object.
		/// @returns An const raw pointer to the game object - which is save, cause of static lifetime of that obj.
		CGame* const game() const { return m_pGame; };

		/// \brief This hooks a component onto this obj.
		/// @param component The enum ID of the component to hook onto.
		void addComponent(components component);
		/// \brief Handle to the kinetics object.
		/// Throws if used without a hooked component.
		/// @returns A smart ptr to the component.
		const std::unique_ptr<CKinetics>& kinetics() const;
		/// \brief Handle to the graphics object.
		/// Throws if used without a hooked component.
		/// @returns A smart ptr to the component.
		const std::unique_ptr<CGraphics>& graphics() const;

		/// \brief Checks if the obj is in the viewable map.
		/// @returns A boolean which is true if in view.
		bool isInView() const;
		/// \brief Helper to check if a given point is in the viewable map.
		/// @param pos The given point to check.
		/// @returns A boolean which is true if in view.
		bool isInView(v2d pos) const;

		/// \brief Helper to determine the factor to diminish the obj near the viewable map.
		/// @returns A float as the factor.
		float fogFactor() const;

	private:
		/// \brief Object position.
		v2d m_pos;
		/// \brief Object color.
		colors m_color;
		/// \brief Object mass.
		int m_mass;
		/// \brief Flag whether the object could be sucked into a black hole.
		bool m_eatable;

		/// \brief Object type.
		objectTypes m_type;
		/// \brief Object state.
		objectStates m_state;

		/// \brief Handle to the central game object.
		CGame* const m_pGame;

		/// \brief Handle to the kinetics component.
		std::unique_ptr<CKinetics> m_pKinetics;
		/// \brief Handle to the graphics component.
		std::unique_ptr<CGraphics> m_pGraphics;
		/// \brief Set of all hooked components.
		std::set<components> m_components;
	};

} //end of namespace bleep