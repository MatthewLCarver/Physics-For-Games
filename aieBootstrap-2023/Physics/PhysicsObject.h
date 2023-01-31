#pragma once
#include <glm/vec2.hpp>

enum ShapeType
{
    PLANE = 0,
    CIRCLE,
    BOX
};

const int SHAPE_COUNT = 3;

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType _shape) : m_shapeID(_shape){}

public:
    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) = 0;
    virtual void Draw(float _alpha) = 0;
    virtual void ResetPosition() {}

    // Getter
    ShapeType GetShapeID() const
    {return m_shapeID;}

protected:
    ShapeType m_shapeID;
    

    
};
