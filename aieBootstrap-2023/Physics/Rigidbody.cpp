#include "Rigidbody.h"

#include <iostream>

#include "PhysicsScene.h"


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

void Rigidbody::ResolveCollision(Rigidbody* actor2)
{
    glm::vec2 normal = glm::normalize(actor2->GetPosition() - m_position);
    glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

    // if the objects are already moving apart, we don't need to do anything
    if (glm::dot(normal, relativeVelocity) >= 0)
        return;

    float elasticity = 1;
    float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
        ((1 / m_mass) + (1 / actor2->GetMass()));

    glm::vec2 force = normal * j;

    float kePre = GetKineticEnergy() + actor2->GetKineticEnergy();
    ApplyForceToActor(actor2, force);
    float kePost = GetKineticEnergy() + actor2->GetKineticEnergy();

    float deltaKE = kePost - kePre;
    if (deltaKE > kePost * 0.01f)
        std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}

float Rigidbody::GetKineticEnergy()
{
    return 0.5f * m_mass * glm::length(m_velocity) * glm::length(m_velocity);
}

float Rigidbody::GetPotentialEnergy()
{
    //return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
    return 0;
}

