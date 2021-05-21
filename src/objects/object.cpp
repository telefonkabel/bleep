//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// object.cpp
/// Abstract class for all game objects.
//==============================================================================

#include "object.h"
#include "exception.h"
#include "game.h"
#include "player.h"
#include "components/graphics.h"
#include "components/kinetics.h"


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
{
}

void CObject::x(float x) { m_pos.x = x; };
void CObject::y(float y) { m_pos.y = y; };
float CObject::x() const { return m_pos.x; };
float CObject::y() const { return m_pos.y; };

void CObject::xy(float x, float y) { m_pos = { x, y }; };
void CObject::xy(v2d point) { m_pos = { point.x, point.y }; };
v2d CObject::xy() const { return m_pos; };

void CObject::color(colors color) { m_color = color; };
colors CObject::color() const { return m_color; };

void CObject::mass(int mass) { m_mass = mass; };
int CObject::mass() const { return m_mass; };

void CObject::state(objectStates state) { m_state = state; };
objectStates CObject::state() const { return m_state; };

objectTypes CObject::type() const { return m_type; };

void CObject::eatable(bool eatable) { m_eatable = eatable; };
bool CObject::isEatable() const { return m_eatable; };

CGame* const CObject::game() const { return m_pGame; };
std::shared_ptr<CPlayer> CObject::player() const { return std::static_pointer_cast<CPlayer>(m_pGame->player()); };


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

std::unique_ptr<CKinetics>& CObject::kinetics()
{
    if (m_pKinetics)
        return m_pKinetics;
    else
        throw CException{ "Tried to grab a nullptr.", INFO };
}

std::unique_ptr<CGraphics>& CObject::graphics()
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
    return (m_pGame->radiusView() - (m_pGame->center() - xy()).mag()) / m_pGame->fog();
}