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
    m_linearDrag = 0.3f;
    m_angularDrag = 0.3f;
    m_elasticity = .3f;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
    CalculateAxes();

    if (length(m_velocity) < MIN_LINEAR_THRESHOLD) 
        m_velocity = glm::vec2(0, 0);
    
    if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) 
        m_angularVelocity = 0;
    
    m_velocity -= m_velocity * m_linearDrag * _timeStep;
    m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;
    m_position += m_velocity * _timeStep;
    m_orientation += m_angularVelocity * _timeStep;
    
    ApplyForce(_gravity * m_mass * _timeStep, glm::vec2(0, 0));
}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
    m_velocity += _force / GetMass();
    m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

void Rigidbody::ApplyForceToActor(Rigidbody* _otherActor, glm::vec2 _force, glm::vec2 _contact)
{
    _otherActor->ApplyForce(_force, _contact);
    ApplyForce(-_force, _contact);
}

void Rigidbody::ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNormal, float _pen)
{
    // find the vector between their centres, or use the provided direction
    // of force, and make sure it's normalised
    glm::vec2 normal = glm::normalize(_collisionNormal ? *_collisionNormal : _actor2->GetPosition() - GetPosition());
    glm::vec2 relativeVelocity = _actor2->GetVelocity() - GetVelocity();
    
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

        // the elasticity is calculated as the average of the two objects
        // CAN EDIT THIS LATER WITH A BETTER EQUATION FOR THE SIMULATION //
        float elasticity = (GetElasticity() + _actor2->GetElasticity()) / 2.0f;

        float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / 
        glm::dot(normal, normal * ((1/m_mass) + (1/_actor2->GetMass())));

        glm::vec2 force = normal * j;
        ApplyForceToActor(_actor2, force, _contact);

        if(_pen > 0)
            PhysicsScene::ApplyContactForces(this, _actor2, normal, _pen);
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
