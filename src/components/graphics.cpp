//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file graphics.cpp
/// \brief Component class which handles all object graphics.
//==============================================================================

#include "graphics.h"
#include "game.h"
#include "jsParser.h"

#include <filesystem>


namespace bleep
{

	//init static member
	bool CGraphics::m_firstInit{ true };
	std::filesystem::path CGraphics::m_graphicPath{};

	CGraphics::CGraphics(CObject* pObject) :
		CComponent{ pObject },
		m_renderable{}
	{
		if (m_firstInit)
		{
			m_graphicPath = { pObject->game()->currentPath() / "graphics" };
			m_firstInit = false;
		}
	}

	void CGraphics::update(float deltaTime)
	{
		draw();
	};

	void CGraphics::sprite(std::string file)
	{
		std::filesystem::path filepath{ m_graphicPath / file };
		if (std::filesystem::exists(filepath))
			m_renderable.Load(filepath.string());
		else
			throw CException{ filepath.string() + " doesn't exist.", INFO };
	}

	void CGraphics::draw() const
	{
		if (object()->isInView())
		{
			v2d v2center{ v2centroid() };
			object()->game()->DrawDecal(object()->xy() - v2center, m_renderable.Decal(), v2d{ 1,1 }, object()->color() * object()->fogFactor());
		}
	}

	v2d CGraphics::v2centroid() const
	{
		return m_renderable.Decal() == nullptr ? object()->xy() : v2d{ static_cast<float>(width()) / 2, static_cast<float>(height()) / 2 };
	}

} //end of namespace bleep