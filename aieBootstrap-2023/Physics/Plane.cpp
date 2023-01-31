﻿#include "Plane.h"

#include <Gizmos.h>
#include <iostream>

#include "Rigidbody.h"

Plane::Plane(glm::vec2 _normal, float _distance) : PhysicsObject(ShapeType::PLANE)
{
    m_distanceToOrigin = _distance;
    m_normal = _normal;
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

void Plane::ResolveCollision(Rigidbody* actor2)
{
    glm::vec2 normal = m_normal;
    glm::vec2 relativeVelocity = actor2->GetVelocity();
    float elasticity = 1;
    
    float j =
        glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
        glm::dot(normal, normal * (1 / actor2->GetMass()));
    
    glm::vec2 force = normal * j;
    actor2->ApplyForce(force);
}

void Plane::ResetPosition()
{
    PhysicsObject::ResetPosition();
}