//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file graphics.h
/// \brief Component class which handles all object graphics.
//==============================================================================

#pragma once
#include "component.h"
#include "objects/object.h"
#include "thirdParty/olcPixelGameEngine.h"

#include <filesystem>
#include <array>


/// \brief Graphics class.
/// This is concrete component class and overwrites the update fct.
class CGraphics : public CComponent
{
public:
	/// \brief Constructor of graphics class.
	/// @param pObject Handle to the corresponding object.
	CGraphics(CObject* pObject);

	/// \brief Update fct.
	/// This is called by the update fct of its object and handles the sprite.
	/// @param deltaTime Elapsed time since last frame.
	void update(float deltaTime) override;

	/// \brief Getter for the sprite width.
	/// @returns The width of the sprite as int, or 0 if no sprite is found.
	int32_t width() const;
	/// \brief Getter for the sprite height.
	/// @returns The height of the sprite as int, or 0 if no sprite is found.
	int32_t height() const;
	/// \brief Inits the graphic of this obj.
	/// @param file Filepath of the graphic.
	void sprite(std::string file);

	/// \brief Draws the obj, if it is in view.
	void draw() const;
	//vector to the centroid of the sprite (rectangular simplification)
	/// \brief Calculates the centroid.
	/// @returns A 2D vector to the centroid of the obj.
	v2d v2centroid() const;

private:
	/// \brief The underlying olc sprite.
	olc::Renderable m_renderable;

	/// \brief Flag to check if this is the first graphics instance.
	static bool m_firstInit;
	/// \brief Path to the graphics folder.
	static std::filesystem::path m_graphicPath;
};