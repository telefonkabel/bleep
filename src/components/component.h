#pragma once
#include "object.h"

//abstract class for components
class CComponent
{
public:
	CComponent(CObject* pObject) :
		m_pObject(pObject)
	{};
	virtual ~CComponent() = default;

	virtual void update(float deltaTime) = 0;
	CObject* object() const { return m_pObject; };

private:
	//pointer to the object which needs to be raw, but is save, cause of komposition
	CObject* m_pObject;
};