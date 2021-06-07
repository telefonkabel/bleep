//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// maths.h
/// Provides often used mathematical calculations.
//==============================================================================

#pragma once
#include "thirdParty/olcPixelGameEngine.h"


using v2d = olc::vf2d;

//some free functions for global mathematical purposes
namespace maths
{

constexpr float PI = 3.1416f;

//for x in [0,1] return y fading in/out in [0,1]
float fadeInOut(float x);

//random angle in radian
float rndAngle();
//random point on circle
v2d rndCirclePt(const v2d& center, float radius);

//rotate a vector around an angle in radian 
void rotate(v2d& vec, float angle);

//rotates the point around the center and projects them onto the circle around the center with the radius if wanted
v2d pointReflection(const v2d& center, const v2d& point, float radius = 0.0f);

//returns the velositiy for disc 1
v2d elasticDiskImpact(v2d& v1, float m1, v2d& v2, float m2);

//orth. projection of v1 onto the target vector
v2d orthProj(v2d& v1, v2d& target);

} //end of namespace maths