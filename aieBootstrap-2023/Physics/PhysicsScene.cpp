#include "PhysicsScene.h"

#include "Circle.h"
#include "PhysicsObject.h"

PhysicsScene::PhysicsScene()
{
    m_timeStep = 0.01f;
    m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
    for (auto pActor : m_actors)
        delete pActor;
}

void PhysicsScene::AddActor(PhysicsObject* _actor)
{
    if(_actor != nullptr)
        m_actors.push_back(_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* _actor)
{
    if(_actor != nullptr)
    {
        for(auto iter = m_actors.begin(); iter != m_actors.end(); ++iter)
        {
            if(*iter == _actor)
            {
                m_actors.erase(iter);
                return;
            }
        }
    }
}

bool PhysicsScene::Circle2Circle(PhysicsObject* _physicsObject1, PhysicsObject* _physicsObject2)
{
    // try to cast objects to Circle and Circle
    Circle* sphere1 = dynamic_cast<Circle*>(_physicsObject1);
    Circle* sphere2 = dynamic_cast<Circle*>(_physicsObject2);
    // if we are successful then test for collision
    if (sphere1 != nullptr && sphere2 != nullptr)
    {
        // TODO do the necessary maths in here
        float positionDistance = glm::distance(sphere1->GetPosition(), sphere2->GetPosition());
        if(positionDistance < (sphere1->GetRadius() + sphere2->GetRadius()))
        {
            // TODO if the Circles touch, set their velocities to zero for now
            /*sphere1->SetVelocity(glm::vec2(0));
            sphere2->SetVelocity(glm::vec2(0));*/
            //sphere1->ApplyForceToActor(sphere2, glm::vec2(1, 0));
            return true;
        }
    }
    return false;
}

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
    PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, 
};

void PhysicsScene::Update(float _dt)
{
    static float accumulatedTime = 0.0f;
    accumulatedTime += _dt;
    while(accumulatedTime > m_timeStep)
    {
        for(auto pActor : m_actors)
        {
            pActor->FixedUpdate(m_gravity, m_timeStep);
        }

        accumulatedTime -= m_timeStep;

        // check for collisions (ideally you'd want to have some sort of 
        // scene management in place)
        int actorCount = m_actors.size();

        //need to check for collisions against all objects except this one.
        for (int outer = 0; outer < actorCount - 1; outer++)
        {
            for (int inner = outer + 1; inner < actorCount; inner++)
            {
                PhysicsObject* object1 = m_actors[outer];
                PhysicsObject* object2 = m_actors[inner];
                int shapeId1 = object1->GetShapeID();
                int shapeId2 = object2->GetShapeID();

                // using function pointers
                int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
                fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
                if (collisionFunctionPtr != nullptr)
                {
                    // did a collision occur?
                    collisionFunctionPtr(object1, object2);        
                }
            }
    }
}

void PhysicsScene::Draw()
{
    for(auto pActor : m_actors)
    {
        pActor->Draw(1);
    }
}
