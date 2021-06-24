//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file hawking.cpp
/// \brief Hawking missile game object.
//==============================================================================

#include "hawking.h"
#include "game.h"
#include "helper/maths.h"
#include "components/graphics.h"
#include "components/kinetics.h"


namespace bleep
{

    bool CHawking::m_firstInit{ true };
    int CHawking::m_radius{};

    CHawking::CHawking(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
        CObject{ pGame, type, pos, mass, color },
        m_direction{}
    {
        if (m_firstInit)
        {
            m_firstInit = false;
            m_radius = pGame->parser().getInt<parser::Missile, parser::Hawking, parser::Radius>();
        }

        addComponent(components::kinetics);

        pGame->sound().playSound(sounds::SHOT0, false);
    }

    CHawking::~CHawking()
    {
        if (isInView() && state() == objectStates::DESTROYED)
            game()->sound().playSound(sounds::IMPACT0, false);
    }

    void CHawking::update(float deltaTime)
    {
        CObject::update(deltaTime);
        calcDirection();

        if (isInView())
        {
            game()->DrawCircle(xy(), m_radius + 1, color() * fogFactor() / 3, m_direction);
            game()->DrawCircle(xy(), m_radius, color() * fogFactor(), m_direction);
            game()->DrawCircle(xy(), m_radius - 1, color() * fogFactor() / 3, m_direction);
        }
    }

    void CHawking::calcDirection()
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

} //end of namespace bleep