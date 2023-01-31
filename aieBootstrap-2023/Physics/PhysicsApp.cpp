#include "PhysicsApp.h"

#include <Gizmos.h>
#include <gl_core_4_4.h>

#include "Demos.h"
#include "Font.h"
#include "Input.h"
#include "Texture.h"

#include <glm/ext.hpp>

#include "Circle.h"
#include "PhysicsScene.h"

PhysicsApp::PhysicsApp()
{
}

PhysicsApp::~PhysicsApp()
{
}

bool PhysicsApp::startup()
{
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);

	DemoStartUp(1);
	
	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float _deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(_deltaTime);
	m_physicsScene->Draw();
	
	m_currentExhaustIncrementTime -= _deltaTime;
	if(m_currentExhaustIncrementTime <= 0.0f)
	{
		DemoUpdate(nullptr, _deltaTime);
		m_currentExhaustIncrementTime = m_exhaustIncrementTime; 
	}

	if(input->isKeyDown(aie::INPUT_KEY_W))
	{
		
	}
	
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.0f/9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>
			(-100, 100,
			-100 / aspectRatio, 100 / aspectRatio,
			-1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartUp(int _num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0, -69));
	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
	
#ifdef NewtonsSecondLaw
	 m_physicsScene->SetGravity(glm::vec2(0, -69));
	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
	
#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-10, 0), glm::vec2(3, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(-3, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForceToActor(ball2, glm::vec2(-2, 0));
#endif

#ifdef SimulatingACollision
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(30, 0));
	ball2->ApplyForce(glm::vec2(-15, 0));

#endif

#ifdef SimulatingRocket
	m_physicsScene->SetGravity(glm::vec2(0, 0));//-GRAVITY));

	static Circle* ball1 = new Circle(glm::vec2(0, 0), glm::vec2(0), 800.0f, 4, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball1);
#endif
}

void PhysicsApp::CullPhysicsActors()
{
	if(m_physicsScene->GetActors().size() > 100)
	{
		// delete second element
		delete m_physicsScene->GetActors()[1];
		m_physicsScene->GetActors().erase(m_physicsScene->GetActors().end() - 1);
	}
}

void PhysicsApp::DemoUpdate(aie::Input* _input, float _dt)
{
#ifdef SimulatingRocket
	Circle* rocketCircle = dynamic_cast<Circle*>(m_physicsScene->GetActors()[0]);
	float M =20.f;
	if(rocketCircle != nullptr && rocketCircle->GetMass() > 0.0f)
	{
		rocketCircle->SetMass(rocketCircle->GetMass() - M);
		if(rocketCircle->GetMass() <= 0.0f)
			return;
		
		Circle* exhaustCircle = new Circle(rocketCircle->GetPosition() + glm::vec2(0, -rocketCircle->GetRadius() - .5f),
			glm::vec2(0), M, M/20,
			glm::vec4(0, 1, 0, 1));
		m_physicsScene->AddActor(exhaustCircle);
		CullPhysicsActors();
		
		rocketCircle->ApplyForceToActor(exhaustCircle, glm::vec2(0, -(M*20)));
		
	}
#endif
	
}

float PhysicsApp::DegreeToRadian(float _degree)
{
	return _degree * (PI / 180.0f); 
}
