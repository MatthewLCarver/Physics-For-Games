#pragma once
#include "PhysicsObject.h"
#include <glm/vec4.hpp>

#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
    Plane(glm::vec2 _normal, float _distance);
    ~Plane() {};

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
    virtual void Draw(float _alpha);
    virtual void ResetPosition();

    //Getters
    glm::vec2 GetNormal() { return m_normal;}
    float GetDistance() {return m_distanceToOrigin;}

    virtual float GetKineticEnergy() { return 0;};
    virtual float GetEnergy() { return 0;};

    void ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact);
protected:
    glm::vec2 m_normal;
    float m_distanceToOrigin;
    glm::vec4 m_color;
    
};
