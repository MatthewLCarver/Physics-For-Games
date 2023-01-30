#include "Circle.h"

#include <Gizmos.h>

Circle::Circle(glm::vec2 _position, glm::vec2 velocity, float _mass, float _radius, glm::vec4 _color) :
Rigidbody(CIRCLE, _position, velocity, 0, _mass)
{
    m_radius = _radius;
    m_color = _color;
}

Circle::~Circle()
{
    
}

void Circle::Draw(float _alpha)
{
    aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_color);
}
