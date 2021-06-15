//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file maths.h
/// \brief Provides often used mathematical calculations.
//==============================================================================

#pragma once
#include "thirdParty/olcPixelGameEngine.h"


/// \brief Typedef for convenience.
using v2d = olc::vf2d;

/// \brief Math namespace to join some free functions together.
namespace maths
{
/// \brief PI - often used and in sufficient accuracy.
constexpr float PI = 3.1416f;

/// \brief Free fct to calculate a smooth in/out fading.
/// This calculates a smooth in/out fading if the input value continuously decelerates from 1 to 0 (or the other way around).
/// It uses the quadratic function: -4x² + 4x
/// @param x Value in [0, 1].
/// @returns A Values in [0, 1].
float fadeInOut(float x);

/// \brief Free fct to calculate a random angle.
/// @returns A random Angle in [0, 2 PI]
float rndAngle();
/// \brief Free fct to calculate a random point on a circle.
/// @param center A 2D vector to the center of the circle.
/// @param radius The radius of the circle.
/// @returns A 2D vector to the calculated point.
v2d rndCirclePt(const v2d& center, float radius);

/// \brief Free fct which rotates an vector by an angle.
/// @param vec The 2D vector to rotate.
/// @param angle The angle in radian by which to rotate.
void rotate(v2d& vec, float angle);

/// \brief Free fct to calc a vector's angle in radian.
/// @param vec A ref to the 2D vector of interest.
/// @returns The angle in radian.
float angle(v2d& vec);
/// \brief Free fct to calc a vector's angle in radian.
/// @param vec The 2D vector of interest.
/// @returns The angle in radian.
float angle(v2d vec);

/// \brief Calculates point reflection.
/// Reflects a given point around the another one (the center) and optionally projects it onto a circle with given radius.
/// @param center The center of the point reflection.
/// @param point The point to reflect.
/// @param radius An optional circle radius to project the reflected point onto.
/// @returns A 2d vector to the reflected point.
v2d pointReflection(const v2d& center, const v2d& point, float radius = -1.0f);

/// \brief Calculates an elastic disk impact between two massive disks for disk1.
/// @param v1 The velocity of disk1.
/// @param m1 The mass of disk1.
/// @param v2 The velocity of disk2.
/// @param m2 The mass of disk2.
/// @returns The calculated velocity of disk1.
v2d elasticDiskImpact(v2d& v1, float m1, v2d& v2, float m2);
/// \brief Calculates an inelastic disk impact between two massive disks.
/// @param v1 The velocity of disk1.
/// @param m1 The mass of disk1.
/// @param v2 The velocity of disk2.
/// @param m2 The mass of disk2.
/// @returns The calculated velocity of the resultant object.
v2d inelasticDiskImpact(v2d& v1, float m1, v2d& v2, float m2);

//orth. projection of v1 onto the target vector

/// \brief Calculates the orthogonal projection of one vector onto another.
/// @param v1 The vector to project.
/// @param target The vector to project onto.
/// @returns The projected vector.
v2d orthProj(v2d& v1, v2d& target);

} //end of namespace maths