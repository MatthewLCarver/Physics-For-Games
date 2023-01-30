#include "Rigidbody.h"


Rigidbody::Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass) :
PhysicsObject(_shapeID)
{
    m_position = _position;
    m_velocity = _velocity;
    m_orientation = _orientation;
    m_mass = _mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
    m_position += m_velocity * _timeStep;
    ApplyForce(_gravity * m_mass * _timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 _force)
{
    m_velocity += _force / m_mass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* _otherActor, glm::vec2 _force)
{
    _otherActor->ApplyForce(_force);
    ApplyForce(-_force);
}


