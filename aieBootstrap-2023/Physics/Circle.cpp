#include "Circle.h"

#include <Gizmos.h>

Circle::Circle(glm::vec2 _position, glm::vec2 velocity, float _mass, float _radius, glm::vec4 _color, bool _isKinematic) :
Rigidbody(CIRCLE, _position, velocity, 0, _mass, _isKinematic)
{
    m_radius = _radius;
    m_color = _color;
    m_moment = 0.5f * m_mass * m_radius * m_radius;
    m_elasticity = 0.70f;
}

Circle::~Circle()
{
    
}

void Circle::Draw(float _alpha)
{
    CalculateSmoothedPosition(_alpha);

    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);

    aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(1,1,1,1));
}

bool Circle::IsInside(glm::vec2 _point)
{
    return glm::distance(_point, m_position) <= m_radius;
}
