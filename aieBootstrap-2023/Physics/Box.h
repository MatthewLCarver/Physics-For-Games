#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
    Box(glm::vec2 _position, glm::vec2 velocity, float _mass, glm::vec2 _extents, glm::vec4 _color, bool _isKinematic, float _orientation, float _elasticity);
    ~Box();
    //virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
    virtual void Draw(float _alpha);
    //bool CheckBoxCorners(const Box& _box, glm::vec2& _contact, int& _numContacts, float& _pen, glm::vec2& _edgeNormal);
    bool CheckBoxCorners(Box& _box, glm::vec2& _contact, int& _numContacts, float& _pen, glm::vec2& _edgeNormal);
    
    // Getters
    glm::vec2 GetExtents() { return m_extents; }
    float GetWidth() { return m_extents.x * 2; }
    float GetHeight() { return m_extents.y * 2; }

protected:
    glm::vec2 m_extents; // the half edge lengths of the box
};
