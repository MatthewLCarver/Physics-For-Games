#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

#include "Demos.h"

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass,  bool _isKinematic);
    virtual ~Rigidbody();
    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
    void ApplyForce(glm::vec2 _force, glm::vec2 _pos);
    void ApplyForceToActor(Rigidbody* _otherActor, glm::vec2 _force, glm::vec2 _contact);

    // Getters
    glm::vec2 GetPosition()
        {return m_position;}
    glm::vec2 GetVelocity()
        {return m_velocity;}
    float GetMass()
        {return m_isKinematic ? INT_MAX : m_mass;}
    float GetOrientation()
        {return m_orientation;}
    glm::vec4 GetColor()
        {return m_color;}
    float GetKineticEnergy()
        {return 0.5f * (m_mass*glm::dot(m_velocity, m_velocity) + 
        m_moment * m_angularVelocity * m_angularVelocity);}
    float GetPotentialEnergy()
        {return m_mass * -GRAVITY * m_position.y;}
    glm::vec2 GetLocalX()
        {return m_localX;}
    glm::vec2 GetLocalY()
        {return m_localY;}
    float GetAngularVelocity()
        {return m_angularVelocity;}
    float GetMoment()
        { return m_isKinematic ? INT_MAX : m_moment; } 
    float GetLinearDrag()
        {return m_linearDrag;}
    float GetAngularDrag()
        {return m_angularDrag;}
    bool IsKinematic()
        { return m_isKinematic; } 
    
    // Setters
    void SetPosition(glm::vec2 _position)
        {m_position = _position;}
    void SetVelocity(glm::vec2 _velocity)
        {m_velocity = _velocity;}
    void SetMass(float _mass)
        {m_mass = _mass;}
    void SetOrientation(float _orientation)
        {m_orientation = _orientation;}
    void SetColor(glm::vec4 _color)
        {m_color = _color;}
    void SetKinematic(bool state)
        { m_isKinematic = state; } 

    void ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr, float _pen = 0);
    virtual void CalculateSmoothedPosition(float _alpha);
    void CalculateAxes();
    glm::vec2 ToWorld(glm::vec2 _contact);
    glm::vec2 ToWorldSmoothed(glm::vec2 _localPos);

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;
    glm::vec2 m_lastPosition;
    float m_lastOrientation;
    
    glm::vec4 m_color;
    
    float m_angularVelocity;
    float m_moment;
    
    glm::vec2 m_smoothedPosition;
    glm::vec2 m_smoothedLocalX;
    glm::vec2 m_smoothedLocalY;

    glm::vec2 m_localX;
    glm::vec2 m_localY;

    float m_linearDrag;
    float m_angularDrag;

    bool m_isKinematic; 
};
