#include "Plane.h"

#include <Gizmos.h>

Plane::Plane(glm::vec2 _normal, float _distance) : PhysicsObject(ShapeType::PLANE)
{
    m_distanceToOrigin = _distance;
    m_normal = glm::vec2(0, 1);
    m_color = glm::vec4(1,0,1,1);
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw(float _alpha)
{
    float lineSegmentLength = 300;
    glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
    // easy to rotate normal through 90 degrees around z
    glm::vec2 parallel(m_normal.y, -m_normal.x);
    glm::vec4 colourFade = m_color;
    colourFade.a = 0;
    glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
    //aie::Gizmos::add2DLine(start, end, colour);
    aie::Gizmos::add2DTri(start, end, start - m_normal*10.0f, m_color, m_color, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colourFade, colourFade);
}

void Plane::ResetPosition()
{
    PhysicsObject::ResetPosition();
}
