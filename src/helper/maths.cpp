//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file maths.cpp
/// \brief Provides often used mathematical calculations.
//==============================================================================

#include "maths.h"


float maths::fadeInOut(float x)
{
    if (x < 0.0f)
        x = 0.0f;
    else if (x > 1.0f)
        x = 1.0f;
    return -4.0f * x * (x - 1.0f);
}

v2d maths::rndCirclePt(const v2d& center, float radius)
{
    float rndAngle(rndAngle());
    return v2d(center + v2d({ radius * static_cast<float>(cos(rndAngle)), radius * static_cast<float>(sin(rndAngle)) }));
}

float maths::rndAngle()
{
    return static_cast<float>(rand()) / RAND_MAX * 2 * PI;
}

v2d maths::pointReflection(const v2d& center, const v2d& point, float radius)
{
    return radius == 0.0f ? (2 * center - point) : (center + (center - point).norm() * radius);
}

v2d maths::elasticDiskImpact(v2d& v1, float m1, v2d& v2, float m2)
{
    v2d v21(v2 - v1);
    v2d v21orth(v21.perp());
    return v2d((2 * m2 * orthProj(v2, v21) + (m1 - m2) * orthProj(v1, v21orth)) / (m1 + m2));
}

v2d maths::orthProj(v2d& v, v2d& target)
{
    float targetMag2(target.mag2());
    if (targetMag2 == 0)
        return v2d();
    return v2d(v.dot(target) * target / target.mag2());
}