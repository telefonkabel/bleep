#include "object.h"
#include "exception.h"

#include "game.h"
#include "components/component.h"
#include "player.h"


CObject::CObject(CGame* const pGame, objectTypes type, v2d pos, int mass, colors color) :
    m_pGame(pGame),
	m_pos(pos),
    m_mass(mass),
	m_color(color),
    m_state(objectStates::ALIVE),
    m_eatable(false),
    m_type(type),
    //makeshift for collsision - has to be implemented properly
    m_pKineticsObj(nullptr),
    m_pSoundsObj(nullptr)
{
}

void CObject::update(float deltaTime)
{
    for (auto& comp : m_pComponents)
        comp->update(deltaTime);
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
std::shared_ptr<CPlayer> CObject::player() const { return std::static_pointer_cast<CPlayer>(game()->player()); };

void CObject::addComponent(std::shared_ptr<CComponent> component)
{ 
    //create a true copy of the shared pointer
    m_pComponents.insert(component); 
};

bool CObject::isInView() const
{
    return ((game()->center() - xy()).mag() < game()->radiusView()) ? true : false;
}
bool CObject::isInView(v2d pos) const
{
    return ((game()->center() - pos).mag() < game()->radiusView()) ? true : false;
}

float CObject::fogFactor() const
{
    return (game()->radiusView() - (game()->center() - xy()).mag()) / game()->fog();
}