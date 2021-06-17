//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file component.h
/// \brief Provides an abstract class for all object components.
//==============================================================================

#pragma once

class CObject;
/// \brief Enum for component types.
enum class components { kinetics, graphics };

/// \brief Basic component class.
/// This is an abstract component class with pure virtual update fct.
class CComponent
{
public:
	/// \brief Constructor of component class.
	/// @param pObject Handle to the corresponding object.
	CComponent(CObject* pObject) : m_pObject{ pObject } {};
	/// \brief Virtual destructor of component class.
	/// Improves code safety for not being able to be called directly.
	virtual ~CComponent() = default;

	/// \brief Pure virtual update fct which needs to be overwritten.
	virtual void update(float deltaTime) = 0;
	/// \brief Handle to the obj.
	/// @returns A raw ptr to the obj: CComponent::m_pObject.
	CObject* object() const { return m_pObject; };

private:
	/// \brief Handle to the object.
	/// This needs to be raw, but is save, cause of composition
	CObject* m_pObject;
};