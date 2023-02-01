#include "Plane.h"

#include <Gizmos.h>
#include <iostream>

#include "Rigidbody.h"

Plane::Plane(glm::vec2 _normal, float _distance) : PhysicsObject(ShapeType::PLANE)
{
    m_distanceToOrigin = _distance;
    m_normal = _normal;
    m_color = glm::vec4(1,0,1,1);
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
}

void Plane::Draw(float _alpha)
{
    float lineSegmentLength = 300;
    glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
    // easy to rotate normal through 90 degrees around z
    glm::vec2 parallel(m_normal.y, -m_normal.x);
    glm::vec4 colourFade = m_color;
    colourFade.a = 0;
    glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
    //aie::Gizmos::add2DLine(start, end, colour);
    aie::Gizmos::add2DTri(start, end, start - m_normal*10.0f, m_color, m_color, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colourFade, colourFade);
}

void Plane::ResolveCollision(Rigidbody* _actor2, glm::vec2 _contact)
{
    // the position at which we'll apply the force relative to the object's COM
    glm::vec2 localContact = _contact - _actor2->GetPosition();

    // the plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
    glm::vec2 vRel = _actor2->GetVelocity() + _actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
    float velocityIntoPlane = glm::dot(vRel, m_normal);

    // perfectly elasticity collisions for now
    float e = 1;

    // this is the perpendicular distance we apply the force at relative to the COM, so Torque = F*r
    float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

    // work out the "effective mass" - this is a combination of moment of
    // inertia and mass, and tells us how much the contact point velocity 
    // will change with the force we're applying
    float mass0 = 1.0f / (1.0f / _actor2->GetMass() + (r * r) / _actor2->GetMoment());

    float j = -(1 + e) * velocityIntoPlane * mass0;

    glm::vec2 force = m_normal * j;

    float kePre = _actor2->GetKineticEnergy();

    _actor2->ApplyForce(force, _contact - _actor2->GetPosition());

    float kePost = _actor2->GetKineticEnergy();

    float deltaKE = kePost - kePre;
    if (deltaKE > kePost * 0.01f)
        std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";
}

void Plane::ResetPosition()
{
    PhysicsObject::ResetPosition();
}
