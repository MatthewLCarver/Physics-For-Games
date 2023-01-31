#pragma once
#include <glm/vec2.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    void AddActor(PhysicsObject* _actor);
    void RemoveActor(PhysicsObject* _actor);

    bool Circle2Circle(PhysicsObject* _physicsObject1, PhysicsObject* _physicsObject2);
    void Update(float _dt);
    void Draw();

    // Setters
    void SetGravity(const glm::vec2 _gravity)
        {m_gravity = _gravity;}
    void SetTimeStep(const float _timeStep)
        {m_timeStep = _timeStep;}

    // Getters
    glm::vec2 GetGravity()
        {return m_gravity;}
    float GetTimeStep()
        {return m_timeStep;}
    std::vector<PhysicsObject*> GetActors()
        {return m_actors;}
    
private:
    glm::vec2 m_gravity;
    float m_timeStep;
    std::vector<PhysicsObject*> m_actors;
    const int SHAPE_COUNT = 2;
    
};
