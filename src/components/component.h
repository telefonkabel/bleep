//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// component.h
/// Provides an abstract class for all object components.
//==============================================================================

#pragma once

class CObject;
enum class components { kinetics, graphics };

class CComponent
{
public:
	CComponent(CObject* pObject) : m_pObject{ pObject } {};
	virtual ~CComponent() = default;

	virtual void update(float deltaTime) = 0;
	CObject* object() const { return m_pObject; };

private:
	//pointer to the object which needs to be raw, but is save, cause of komposition
	CObject* m_pObject;
};