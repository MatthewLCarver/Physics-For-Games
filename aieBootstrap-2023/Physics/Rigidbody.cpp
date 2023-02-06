#include "Rigidbody.h"

#include <iostream>
#include <glm/gtc/constants.hpp>

#include "PhysicsScene.h"


Rigidbody::Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass,
                     bool _isKinematic = false) :
PhysicsObject(_shapeID)
{
    m_position = _position;
    m_velocity = _velocity;
    m_orientation = _orientation;
    m_mass = _mass;
    m_lastOrientation = _orientation;
    m_angularVelocity = 0;
    m_lastPosition = _position;
    m_linearDrag = 0.3f;
    m_angularDrag = 0.3f;
    m_elasticity = 0.3f;
    m_isKinematic = _isKinematic;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
    CalculateAxes();

    if(m_isKinematic)
    {
        m_velocity = glm::vec2(0);
        m_angularVelocity = 0;
        return;
    }

    m_lastPosition = m_position;
    m_position += m_velocity * _timeStep;
    
    ApplyForce(_gravity * m_mass * _timeStep, glm::vec2(0));

    m_orientation += m_angularVelocity * _timeStep;
    m_velocity -= m_velocity * m_linearDrag * _timeStep;
    m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;

    if(length(m_velocity) < MIN_LINEAR_THRESHOLD)
    {
        m_velocity = glm::vec2(0);
    }
    if(abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
    {
        m_angularVelocity = 0;
    }
}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
    m_velocity += _force / GetMass();
    m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

void Rigidbody::ApplyForceToActor(Rigidbody* _actor2, glm::vec2 _force, glm::vec2 _contact)
{
    _actor2->ApplyForce(_force, _contact);
    ApplyForce(-_force, _contact);
}

void Rigidbody::ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNormal, float _pen)
{
    glm::vec2 normal = glm::normalize(_collisionNormal ? *_collisionNormal : _actor2->m_position - m_position);
    glm::vec2 perp(-normal.y, normal.x);
    glm::vec2 relativeVelocity = _actor2->GetVelocity() - m_velocity;

    float r1 = glm::dot(_contact - m_position, -perp);
    float r2 = glm::dot(_contact - _actor2->m_position, perp);

    float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;

    float v2 = glm::dot(_actor2->m_velocity, normal) + r2 * _actor2->m_angularVelocity;

    if(v1 > v2)
    {
        float mass1 = 1.f / (1.f / GetMass() + (r1 * r1) / GetMoment());
        float mass2 = 1.f / (1.f / _actor2->GetMass() + (r2 * r2) / _actor2->GetMoment());

        float elasticity = (GetElasticity() + _actor2->GetElasticity()) / 2.0f;

        float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
            glm::dot(normal, normal * ((1 / GetMass()) + (1 / _actor2->GetMass())));

        glm::vec2 force = normal * j;

        ApplyForceToActor(_actor2, force, _contact);
    }
    if(_pen > 0)
        PhysicsScene::ApplyContactForces(this, _actor2, normal, _pen);
}

void Rigidbody::CalculateSmoothedPosition(float _alpha)
{
    m_smoothedPosition = _alpha * m_position + (1 - _alpha) * m_lastPosition;

    float smoothedOrientation = _alpha * m_orientation + (1 - _alpha) * m_lastOrientation;
    float sn = sinf(smoothedOrientation);
    float cs = cosf(smoothedOrientation);
    
    m_smoothedLocalX = glm::vec2(cs, sn);
    m_smoothedLocalY = glm::vec2(-sn, cs);

    m_lastPosition = m_position;
    m_lastOrientation = m_orientation;
}

void Rigidbody::CalculateAxes()
{
    float sn = sinf(m_orientation);
    float cs = cosf(m_orientation);
    m_localX = glm::vec2(cs, sn);
    m_localY = glm::vec2(-sn, cs);
}

glm::vec2 Rigidbody::ToWorld(glm::vec2 _contact)
{
    glm::vec2 worldPos = m_position + m_localX * _contact.x + m_localY * _contact.y;
    return worldPos;
}

glm::vec2 Rigidbody::ToWorldSmoothed(glm::vec2 _localPos)
{
    glm::vec2 worldPos = m_smoothedPosition + m_smoothedLocalX * _localPos.x + m_smoothedLocalY * _localPos.y;
    return worldPos;
}