//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// graphics.cpp
/// Component class which handles all object graphics.
//==============================================================================

#include "graphics.h"
#include "game.h"
#include "jsParser.h"

#include <filesystem>


//init static member
bool CGraphics::m_firstInit{ true };
std::filesystem::path CGraphics::m_graphicPath{};
std::array<std::vector<std::filesystem::path>, static_cast<int>(objectTypes::count)> CGraphics::m_graphics
	{ std::array<std::vector<std::filesystem::path>, static_cast<int>(objectTypes::count)>() };

CGraphics::CGraphics(CObject* pObject) :
	CComponent{ pObject },
	m_renderable{}
{
	if (m_firstInit)
	{
		m_graphicPath = { pObject->game()->currentPath() / "graphics" };
		std::cout << std::endl << "Initialize sprites out of " << m_graphicPath << std::endl;
		initGraphics();
		m_firstInit = false;
		std::cout << "All sprites successfully initialized." << std::endl;
	}
}

void CGraphics::initGraphics()
{
	std::vector<std::string> files{ object()->game()->parser().getVString<parser::Debris, parser::Asteroid, parser::Sprite>() };
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).resize(static_cast<int>(sprites::count));
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(sprites::ASTR_SMALL1)) = m_graphicPath / files[0];
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(sprites::ASTR_SMALL2)) = m_graphicPath / files[1];
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(sprites::ASTR_MEDIUM)) = m_graphicPath / files[2];
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(sprites::ASTR_BIG)) = m_graphicPath / files[3];
}

void CGraphics::update(float deltaTime)
{
	draw();
}

void CGraphics::sprite(objectTypes type, sprites gfx)
{
	std::filesystem::path file{ m_graphics.at(static_cast<int>(type)).at(static_cast<int>(gfx)) };
	if (std::filesystem::exists(file))
		m_renderable.Load(file.string());
	else
		throw CException{ file.string() + " doesn't exist.", INFO };
}

bool CGraphics::draw() const
{
	if (!object()->isInView())
		return true;
	v2d v2center{ v2centroid() };
	object()->game()->DrawDecal(object()->xy() - v2center, m_renderable.Decal(), v2d{ 1,1 }, object()->color() * std::min(object()->fogFactor(), 1.0f));

	return true;
}

v2d CGraphics::v2centroid() const
{
	return m_renderable.Decal() == nullptr ? object()->xy() : v2d{ static_cast<float>(width()) / 2, static_cast<float>(height()) / 2 };
}

int32_t CGraphics::width() const { return m_renderable.Sprite()->width; };
int32_t CGraphics::height() const { return m_renderable.Sprite()->height; };