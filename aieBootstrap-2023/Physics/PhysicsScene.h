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

    static bool Circle2Circle(PhysicsObject* _circle1, PhysicsObject* _circle2);
    static bool Circle2Plane(PhysicsObject* _circle, PhysicsObject* _plane);
    static bool Circle2Box(PhysicsObject* _circle, PhysicsObject* _box);

    static bool Plane2Plane(PhysicsObject* _plane1, PhysicsObject* _plane2);
    static bool Plane2Circle(PhysicsObject* _plane, PhysicsObject* _circle);
    static bool Plane2Box(PhysicsObject* _plane, PhysicsObject* _box);

    static bool Box2Box(PhysicsObject* _box1, PhysicsObject* _box2);
    static bool Box2Plane(PhysicsObject* _box, PhysicsObject* _plane);
    static bool Box2Circle(PhysicsObject* _box, PhysicsObject* _circle);
    
    
    void Update(float _dt);
    void Draw();

    void CheckForCollision();

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
    
};
