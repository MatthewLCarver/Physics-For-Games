#pragma once

#include <vector>
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
 
public:
	void DemoStartUp(int _num);
	void DemoUpdate(aie::Input* _input, float _dt);
	float DegreeToRadian(float _degree);

private:
	float m_exhaustIncrementTime = 0.25f;
	float m_currentExhaustIncrementTime = 0.25f;
};