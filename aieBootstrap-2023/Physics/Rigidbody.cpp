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
    m_lastOrientation = _orientation;
    m_angularVelocity = 0;
    m_lastPosition = _position;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
    CalculateAxes();
    m_position += m_velocity * _timeStep;
    m_orientation += m_angularVelocity * _timeStep;
    
    ApplyForce(_gravity * m_mass * _timeStep, glm::vec2(0, 0));
}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
    m_velocity += _force / GetMass();
    m_angularVelocity += 0.0f;//(_force.y * m_position.x - _force.x * m_position.y) / GetMoment();
}

/*void Rigidbody::ApplyForceToActor(Rigidbody* _otherActor, glm::vec2 _force)
{
    _otherActor->ApplyForce(_force);
    ApplyForce(-_force);
}*/

void Rigidbody::ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNormal)
{
    // find the vector between their centres, or use the provided direction
    // of force, and make sure it's normalised
    glm::vec2 normal = glm::normalize(_collisionNormal ? *_collisionNormal :
        _actor2->m_position - m_position);
    // get the vector perpendicular to the collision normal
    glm::vec2 perp(normal.y, -normal.x);

    // determine the total velocity of the contact points for the two objects, 
    // for both linear and rotational

    // 'r' is the radius from axis to application of force
    float r1 = glm::dot(_contact - m_position, -perp);		
    float r2 = glm::dot(_contact - _actor2->m_position, perp);		
    // velocity of the contact point on this object 
    float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
    // velocity of contact point on actor2
    float v2 = glm::dot(_actor2->m_velocity, normal) + 
    r2 * _actor2->m_angularVelocity;
    if (v1 > v2) // they're moving closer
    {
        // calculate the effective mass at contact point for each object
        // ie how much the contact point will move due to the force applied.
        float mass1 = 1.0f / (1.0f / m_mass + (r1*r1) / m_moment);
        float mass2 = 1.0f / (1.0f / _actor2->m_mass + (r2*r2) / _actor2->m_moment);

        float elasticity = 1;

        glm::vec2 force = (1.0f + elasticity)*mass1*mass2 / 
        (mass1 + mass2)*(v1 - v2)*normal;

        //apply equal and opposite forces
        ApplyForce(-force, _contact - m_position);
        _actor2->ApplyForce(force, _contact - _actor2->m_position);
    }	
}

void Rigidbody::CalculateSmoothedPosition(float alpha)
{
    m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

    float smoothedOrientation = alpha * m_orientation + (1 - alpha) * m_lastOrientation;

    float cs = cosf(smoothedOrientation);
    float sn = sinf(smoothedOrientation);

    m_smoothedLocalX = glm::vec2( cs, sn);
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
