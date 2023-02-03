#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Spring : public PhysicsObject
{
public:
    Spring(Rigidbody* _body1, Rigidbody* _body2, 
    float _springCoefficient, float _damping, float _restLength,
    glm::vec2 _contact1=glm::vec2(0, 0), glm::vec2 _contact2=glm::vec2(0, 0) );
    ~Spring();

    virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
    virtual void Draw(float _alpha);
    glm::vec2 GetContact1();
    glm::vec2 GetContact2();
    
    Rigidbody* m_body1;
    Rigidbody* m_body2;

    glm::vec2 m_contact1;
    glm::vec2 m_contact2;

    glm::vec4 m_color;

    float m_damping;
    float m_restLength;
    float m_springCoefficient;		// the restoring force;
};
