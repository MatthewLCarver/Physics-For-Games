#pragma once

#include <vector>
#include <glm/detail/type_vec.hpp>

#include "Application.h"
#include "Input.h"
#include "Renderer2D.h"

class PhysicsScene;
class PhysicsObject;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float _deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	std::vector<PhysicsObject*> m_objects;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;
 
public:
	void DemoStartUp(int _num);
	void DemoUpdate(aie::Input* _input, float _dt);
	float DegreeToRadian(float _degree);
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);

private:
	float m_exhaustIncrementTime = 0.25f;
	float m_currentExhaustIncrementTime = 0.25f;
};