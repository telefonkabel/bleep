//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// graphics.h
/// Component class which handles all object graphics.
//==============================================================================

#pragma once
#include "component.h"
#include "objects/object.h"
#include "thirdParty/olcPixelGameEngine.h"

#include <filesystem>
#include <array>


class CGraphics : public CComponent
{
public:
	CGraphics(CObject* pObject);

	void update(float deltaTime) override;

	int32_t width() const;
	int32_t height() const;
	void sprite(objectTypes type, std::string file);

	bool draw() const;
	//vector to the centroid of the sprite (rectangular simplyfication)
	v2d v2centroid() const;

private:
	olc::Renderable m_renderable;

	////encapsulates graphics in this component and shares them between all instances
	static bool m_firstInit;
	static std::filesystem::path m_graphicPath;
};