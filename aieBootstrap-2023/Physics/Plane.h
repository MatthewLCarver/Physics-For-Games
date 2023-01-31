#pragma once
#include <glm/glm.hpp>
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
    Plane(glm::vec2 _normal, float distance);
    ~Plane();

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
    virtual void Draw(float _alpha);
    void ResolveCollision(Rigidbody* actor2);
    virtual void ResetPosition();

    glm::vec2 GetNormal() { return m_normal; }
    float GetDistance() { return m_distanceToOrigin; }

protected:
    glm::vec2 m_normal;
    float m_distanceToOrigin;
    glm::vec4 m_color;
};
