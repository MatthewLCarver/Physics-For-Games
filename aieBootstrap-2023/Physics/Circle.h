#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
    Circle(glm::vec2 _position, glm::vec2 velocity, float _mass, float _radius, glm::vec4 _color);
    ~Circle();

    virtual void Draw(float _alpha);

    // Getter
    float GetRadius()
        {return m_radius;}
    
    // Setter
    void SetRadius (float _radius)
        {m_radius = _radius;}
    
protected:
    float m_radius;
};
