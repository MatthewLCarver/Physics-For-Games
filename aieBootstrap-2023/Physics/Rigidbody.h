#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass);
    virtual ~Rigidbody();
    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
    void ApplyForce(glm::vec2 _force);
    void ApplyForceToActor(Rigidbody* _otherActor, glm::vec2 _force);

    // Getters
    glm::vec2 GetPosition()
        {return m_position;}
    glm::vec2 GetVelocity()
        {return m_velocity;}
    float GetMass()
        {return m_mass;}
    float GetOrientation()
        {return m_orientation;}
    glm::vec4 GetColor()
        {return m_color;}
    
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
    

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;

    glm::vec4 m_color;
};
