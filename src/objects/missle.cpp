//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// missle.cpp
/// Missle game object.
//==============================================================================

#include "missle.h"
#include "game.h"
#include "helper/maths.h"
#include "components/graphics.h"
#include "components/kinetics.h"


CMissle::CMissle(CGame* const pGame, objectTypes type, int mass, v2d pos, colors color, int radius) :
    CObject{ pGame, type, pos, mass, color },
    m_radius{ radius },
    m_direction{}
{
    addComponent(components::kinetics);
    eatable(true);

    pGame->sound().playSound(sounds::SHOT0, false);
}

CMissle::~CMissle()
{
    if (isInView() && state() == objectStates::DESTROYED)
        game()->sound().playSound(sounds::IMPACT0, false);
}

void CMissle::update(float deltaTime)
{
    CObject::update(deltaTime);
    calcDirection();

    if (isInView())
    {
        game()->DrawCircle(xy(), m_radius + 1, color() * fogFactor() / 3, m_direction);
        game()->DrawCircle(xy(), m_radius, color() * fogFactor(), m_direction);
        game()->DrawCircle(xy(), m_radius - 1, color() * fogFactor() / 3, m_direction);
    }

    //debug
    if (kinetics()->speed() < 300)
    {
        auto sp = kinetics()->speed();
        auto test = type();
    }
}

float CMissle::edge()
{
    return m_radius;
}

void CMissle::calcDirection()
{
    float angle{ maths::angle(kinetics()->velocity()) };
    float step{ maths::PI / 8 };

    if (angle <= step * (-1) && angle > step * (-3))
        m_direction = static_cast<uint8_t>(octant::Q11) + static_cast<uint8_t>(octant::Q12);
    else if (angle <= step * (-3) && angle > step * (-5))
        m_direction = static_cast<uint8_t>(octant::Q12) + static_cast<uint8_t>(octant::Q21);
    else if (angle <= step * (-5) && angle > step * (-7))
        m_direction = static_cast<uint8_t>(octant::Q21) + static_cast<uint8_t>(octant::Q22);
    else if (angle <= step * (-7) || angle > step * 7)
        m_direction = static_cast<uint8_t>(octant::Q22) + static_cast<uint8_t>(octant::Q31);
    else if (angle <= step * 7 && angle > step * 5)
        m_direction = static_cast<uint8_t>(octant::Q31) + static_cast<uint8_t>(octant::Q32);
    else if (angle <= step * 5 && angle > step * 3)
        m_direction = static_cast<uint8_t>(octant::Q32) + static_cast<uint8_t>(octant::Q41);
    else if (angle <= step * 3 && angle > step)
        m_direction = static_cast<uint8_t>(octant::Q41) + static_cast<uint8_t>(octant::Q42);
    else
        m_direction = static_cast<uint8_t>(octant::Q42) + static_cast<uint8_t>(octant::Q11);
}