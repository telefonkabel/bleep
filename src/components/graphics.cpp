//==============================================================================
/// 2021, Benedikt Michael.
//==============================================================================
/// graphics.cpp
/// Component class which handles all object graphics.
//==============================================================================

#include "graphics.h"


//init static member
bool CGraphics::m_firstInit = true;
std::array<std::vector<std::filesystem::path>, static_cast<int>(objectTypes::count)> CGraphics::m_graphics = std::array<std::vector<std::filesystem::path>, static_cast<int>(objectTypes::count)>();

CGraphics::CGraphics(CObject* pObject) :
	CComponent(pObject),
	m_pSprite(nullptr),
	m_pDecal(nullptr),
	m_graphicPath(pObject->game()->currentPath() / "graphics")
{
	if (m_firstInit)
		initGraphics();
}


CGraphics::~CGraphics()
{
	delete m_pSprite;
	delete m_pDecal;
}

void CGraphics::initGraphics()
{
	m_firstInit = false;
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).resize(static_cast<int>(graphics::count));
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(graphics::ASTR_SMALL1)) = m_graphicPath / "astr0.png";
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(graphics::ASTR_SMALL2)) = m_graphicPath / "astr1.png";
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(graphics::ASTR_MEDIUM)) = m_graphicPath / "astr2.png";
	m_graphics.at(static_cast<int>(objectTypes::DEBRIS)).at(static_cast<int>(graphics::ASTR_BIG)) = m_graphicPath / "astr3.png";
}

void CGraphics::update(float deltaTime)
{
	draw();
}

void CGraphics::sprite(objectTypes type, graphics gfx)
{
	std::string file = m_graphics.at(static_cast<int>(type)).at(static_cast<int>(gfx)).string();
	if (!file.empty())
	{
		if (std::filesystem::exists(file))
		{
			olc::Sprite* m_pSprite(new olc::Sprite(file));
			m_pDecal = new olc::Decal(m_pSprite);
		}
		else
			; //throw
	}
}

bool CGraphics::draw() const
{
	if (!object()->isInView())
		return true;
	v2d v2center(v2centroid());
	object()->game()->DrawDecal(object()->xy() - v2center, m_pDecal, v2d({ 1,1 }), object()->color() * std::min(object()->fogFactor(), 1.0f));

	return true;
}

v2d CGraphics::v2centroid() const
{
	return m_pDecal == nullptr ? object()->xy() : v2d{ static_cast<float>(m_pDecal->sprite->width) / 2, static_cast<float>(m_pDecal->sprite->height) / 2 };
}

int32_t CGraphics::width() const { return m_pDecal->sprite->width; };
int32_t CGraphics::height() const { return m_pDecal->sprite->height; };