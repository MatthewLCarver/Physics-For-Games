#pragma once
#include <glm/glm.hpp>
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
    Plane(glm::vec2 _normal, float _distance);
    ~Plane();

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
    virtual void Draw(float _alpha);
    void ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact);
    virtual void ResetPosition();

    glm::vec2 GetNormal() { return m_normal; }
    float GetDistance() { return m_distanceToOrigin; }

    virtual float GetEnergy() override
        { return 0;}
    virtual float GetKineticEnergy() override
        { return 0;}

protected:
    glm::vec2 m_normal;
    float m_distanceToOrigin;
    glm::vec4 m_color;
};
