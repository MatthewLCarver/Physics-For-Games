#pragma once
#include <string>

#include "PhysicsScene.h"

class SoftBody
{
public:
    static void BuildCircles(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& strings);
    static void BuildBoxes(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& strings);
};
