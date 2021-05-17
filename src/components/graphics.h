//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file graphics.h
/// \brief Component class which handles all object graphics.
//==============================================================================

#pragma once
#include "component.h"
#include "game.h"


enum class graphics { ASTR_SMALL1, ASTR_SMALL2, ASTR_MEDIUM, ASTR_BIG, count };

class CGraphics : public CComponent
{
public:
	CGraphics(CObject* pObject);
	~CGraphics();

	void update(float deltaTime) override;

	int32_t width() const;
	int32_t height() const;
	void sprite(objectTypes type, graphics gfx);

	bool draw() const;
	v2d v2centroid() const;

private:
	olc::Sprite* m_pSprite;
	olc::Decal* m_pDecal;

	std::filesystem::path m_graphicPath;

	////encapsulates graphics in this component and shares them between all instances
	static bool m_firstInit;
	static std::array<std::vector<std::filesystem::path>, static_cast<int>(objectTypes::count)> m_graphics;

	void initGraphics();
};