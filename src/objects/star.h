//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file star.h
/// \brief Background star object.
//==============================================================================

#pragma once
#include "object.h"


class CGame;

/// \brief Star class.
/// This is a concrete object class and overwrites the edge and update fct.
class CStar : public CObject
{
public:
	/// \brief Constructor of star class.
	/// @param pGame Handle to the central game object.
	/// @param type Its object type.
	/// @param pos Its start position.
	/// @param mass Its start mass.
	/// @param color Its color.
	CStar(CGame* const pGame, objectTypes type, v2d pos = { 0.0f, 0.0f }, int mass = 0, colors color = olc::WHITE);

	/// \brief Update fct.
	/// This is called by the game update fct and calls the update fct of its base class.
	/// @param deltaTime Elapsed time since last frame.
	void update(float deltaTime) override;
	/// \brief Calculates the collision boundary of the obj.
	/// This overwrites the edge fct of its base class, but for background stars this hasn't any importance.
	/// @returns 0, cause background stars have no edge.
	float edge() override;

private:
	/// \brief Duration of the flicker effect.
	/// This gets steadily counted down till it reaches 0, then it gets set back to the effect time.
	float m_flickerEffect;
	/// \brief Duration of the flicker effect.
	float m_flickerEffectTime;
	/// \brief Small delay between the chances to flicker.
	/// This gets steadily counted down till it reaches 0, then it gets set back to the reload time.
	float m_flickerTimer;
	/// \brief Small delay between the chances to flicker.
	float m_flickerTimerReload;

	/// \brief Draws the star.
	/// This takes random flickering and lens effects into account.
	void draw() const;
};