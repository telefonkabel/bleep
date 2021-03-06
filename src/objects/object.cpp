//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file object.cpp
/// \brief Abstract class for all game objects.
//==============================================================================

#include "object.h"
#include "blackHole.h"
#include "game.h"
#include "helper/exception.h"
#include "components/graphics.h"
#include "components/kinetics.h"


namespace bleep
{

    CObject::CObject(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
        m_pGame{ pGame },
        m_pos{ pos },
        m_mass{ mass },
        m_color{ color },
        m_state{ objectStates::ALIVE },
        m_eatable{ false },
        m_type{ type },
        m_pKinetics{},
        m_pGraphics{}
    {}

    void CObject::update(float deltaTime)
    {
        for (components component : m_components)
        {
            switch (component)
            {
            case components::kinetics:
                m_pKinetics->update(deltaTime);
                break;
            case components::graphics:
                m_pGraphics->update(deltaTime);
                break;
            default:
                throw CException{ "Case for component with enum value " + std::to_string(static_cast<int>(component)) + " has to be implemented here.", INFO };
                break;
            }
        }
    }

    void CObject::addComponent(components component)
    {
        m_components.insert(component);
        switch (component)
        {
        case components::kinetics:
            m_pKinetics = std::make_unique<CKinetics>(this);
            break;
        case components::graphics:
            m_pGraphics = std::make_unique<CGraphics>(this);
            break;
        default:
            throw CException{ "Case for component with enum value " + std::to_string(static_cast<int>(component)) + " has to be implemented here.", INFO };
            break;
        }
    };

    const std::unique_ptr<CKinetics>& CObject::kinetics() const
    {
        if (m_pKinetics)
            return m_pKinetics;
        else
            throw CException{ "Tried to grab a nullptr.", INFO };
    }

    const std::unique_ptr<CGraphics>& CObject::graphics() const
    {
        if (m_pGraphics)
            return m_pGraphics;
        else
            throw CException{ "Tried to grab a nullptr.", INFO };
    }

    bool CObject::isInView() const
    {
        return ((m_pGame->center() - xy()).mag() < m_pGame->radiusView()) ? true : false;
    }
    bool CObject::isInView(v2d pos) const
    {
        return ((m_pGame->center() - pos).mag() < m_pGame->radiusView()) ? true : false;
    }

    float CObject::fogFactor() const
    {
        return std::min((m_pGame->radiusView() - (m_pGame->center() - xy()).mag()) / m_pGame->fog(), 1.0f);
    }

} //end of namespace bleep