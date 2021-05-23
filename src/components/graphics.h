//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// graphics.h
/// Component class which handles all object graphics.
//==============================================================================

#pragma once
#include "component.h"
#include "game.h"


enum class sprites { ASTR_SMALL1, ASTR_SMALL2, ASTR_MEDIUM, ASTR_BIG, count };

class CGraphics : public CComponent
{
public:
	CGraphics(CObject* pObject);

	void update(float deltaTime) override;

	int32_t width() const;
	int32_t height() const;
	void sprite(objectTypes type, sprites gfx);

	bool draw() const;
	//vector to the centroid of the sprite (rectangular simplyfication)
	v2d v2centroid() const;

private:
	olc::Renderable m_renderable;

	////encapsulates graphics in this component and shares them between all instances
	static bool m_firstInit;
	static std::filesystem::path m_graphicPath;
	static std::array<std::vector<std::filesystem::path>, static_cast<int>(objectTypes::count)> m_graphics;

	void initGraphics();
};