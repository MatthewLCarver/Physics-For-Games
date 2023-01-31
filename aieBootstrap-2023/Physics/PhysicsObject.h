#pragma once
#include <glm/vec2.hpp>

enum ShapeType
{
    PLANE = 0,
    CIRCLE//,
    //BOX
};

class PhysicsObject
{
protected:
    PhysicsObject(ShapeType _shape);

public:
    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) = 0;
    virtual void Draw(float _alpha) = 0;
    virtual void ResetPosition() {}

    // Getter
    ShapeType GetShapeID()
        {return m_shapeID;}

protected:
    ShapeType m_shapeID;
    

    
};
