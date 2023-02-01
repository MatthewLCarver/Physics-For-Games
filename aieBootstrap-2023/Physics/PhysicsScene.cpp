#include "PhysicsScene.h"

#include "Box.h"
#include "Circle.h"
#include "PhysicsObject.h"
#include "Plane.h"

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
    // DONE \\
    
    // try to cast objects to Circle and Circle
    Circle* circle1 = dynamic_cast<Circle*>(_physicsObject1);
    Circle* circle2 = dynamic_cast<Circle*>(_physicsObject2);
    // if we are successful then test for collision
    if (circle1 != nullptr && circle2 != nullptr)
    {
        // TODO do the necessary maths in here
        float positionDistance = glm::distance(circle1->GetPosition(), circle2->GetPosition());
        if(positionDistance < (circle1->GetRadius() + circle2->GetRadius()))
        {
            // TODO if the Circles touch, set their velocities to zero for now
            circle1->ResolveCollision(circle2, 0.5f * (circle1->GetPosition() + circle2->GetPosition()));
            return true;
        }
    }
    return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* _circle, PhysicsObject* _plane)
{
    // DONE \\
    
    Circle* circle = dynamic_cast<Circle*>(_circle);
    Plane* plane = dynamic_cast<Plane*>(_plane);
    //if we are successful then test for collision
    if (circle != nullptr && plane != nullptr)
    {
        glm::vec2 collisionNormal = plane->GetNormal();
        float sphereToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

        float intersection = circle->GetRadius() - sphereToPlane;
        float velocityOutOfPlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
        if (intersection > 0 && velocityOutOfPlane < 0)
        {
            glm::vec2 contact = circle->GetPosition() + (collisionNormal * -circle->GetRadius());
            //set Circle velocity to zero here
            plane->ResolveCollision(circle, contact);
            return true;
        }
    }
    return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject* _circle, PhysicsObject* _box)
{
    // DONE \\
    
    return Box2Circle(_box, _circle);
}

bool PhysicsScene::Plane2Plane(PhysicsObject* _plane1, PhysicsObject* _plane2)
{
    // DONE \\
    
    return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* _plane, PhysicsObject* _circle)
{
    // DONE \\
    
    return Circle2Plane(_circle, _plane);
}

bool PhysicsScene::Plane2Box(PhysicsObject* _plane, PhysicsObject* _box)
{
    // DONE \\
    
    Plane* plane = dynamic_cast<Plane*>(_plane);
    Box* box = dynamic_cast<Box*>(_box);

    //if we are successful then test for collision
    if (box != nullptr && plane != nullptr)
    {
        int numContacts = 0;
        glm::vec2 contact(0, 0);
        float contactV = 0;

        // Get a representative point on the plane
        glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

        // check all four corners to see if we've hit the plane
        for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
        {
            for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
            {
                // Get the position of the corner in world space
                glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
                float distFromPlane = glm::dot(p - planeOrigin, plane->GetNormal());

                // this is the total velocity of the point in world space
                glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
                glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
                // and this is the component of the point velocity into the plane
                float velocityIntoPlane = glm::dot(pointVelocity, plane->GetNormal());

                // and moving further in, we need to resolve the collision
                if (distFromPlane < 0 && velocityIntoPlane <= 0)
                {
                    numContacts++;
                    contact += p;
                    contactV += velocityIntoPlane;
                }
            }
        }

        // we've had a hit - typically only two corners can contact
        if (numContacts > 0)
        {
            plane->ResolveCollision(box, contact / (float)numContacts);
            return true;
        }
    }

    return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* _box1, PhysicsObject* _box2)
{
    Box* box1 = dynamic_cast<Box*>(_box1);
    Box* box2 = dynamic_cast<Box*>(_box2);
    if (box1 != nullptr && box2 != nullptr) {
        glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
        glm::vec2 norm(0, 0);
        glm::vec2 contact(0, 0);
        float pen = 0;
        int numContacts = 0;
        box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
        if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm)) {
            norm = -norm;
        }
        if (pen > 0) {
            box1->ResolveCollision(box2, contact / float(numContacts), &norm);
        }
        return true;
    }
    return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* _box, PhysicsObject* _plane)
{
    // DONE \\
    
    return Plane2Box(_plane, _box);
}

bool PhysicsScene::Box2Circle(PhysicsObject* _box, PhysicsObject* _circle)
{
    // DONE \\
    
    Box* box = dynamic_cast<Box*>(_box);
    Circle* circle = dynamic_cast<Circle*>(_circle);

    if (box != nullptr && circle != nullptr) 
    {
        // transform the circle into the box's coordinate space
        glm::vec2 circlePosWorld = circle->GetPosition() - box->GetPosition();
        glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));

        // find the closest point to the circle centre on the box by clamping the coordinates in box-space to the box's extents
        glm::vec2 closestPointOnBoxBox = circlePosBox;
        glm::vec2 extents = box->GetExtents();
        if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
        if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
        if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
        if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;
        // and convert back into world coordinates
        glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x * box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
        glm::vec2 circleToBox = circle->GetPosition() - closestPointOnBoxWorld;
        if (glm::length(circleToBox)< circle->GetRadius())
        {
            glm::vec2 direction = glm::normalize(circleToBox);
            glm::vec2 contact = closestPointOnBoxWorld;
            box->ResolveCollision(circle, contact, &direction);
        }
    }

    return false;
}

// function pointer array for doing our collisions
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::Plane2Plane,  PhysicsScene::Plane2Circle,  PhysicsScene::Plane2Box,
    PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Box,
    PhysicsScene::Box2Plane,    PhysicsScene::Box2Circle,    PhysicsScene::Box2Box,
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
}

void PhysicsScene::Draw()
{
    for(auto pActor : m_actors)
    {
        pActor->Draw(1);
    }
}

void PhysicsScene::CheckForCollision()
{
}
