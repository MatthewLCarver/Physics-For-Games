#include "Application2D.h"

#include <Box.h>
#include <Circle.h>
#include <Demos.h>
#include <iostream>
//#include <Plane.h>

#include "Font.h"
#include "Input.h"
#include "Texture.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include <Gizmos.h>
#include <glm/gtc/matrix_transform.hpp>

Application2D::Application2D()
{
	
}

Application2D::~Application2D()
{
	delete m_backgroundTexture;
	delete m_tableTexture;
}

bool Application2D::Startup() {

	//Increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_physicsScene = new PhysicsScene();
	
	m_2dRenderer = new aie::Renderer2D();

	m_texture = new aie::Texture("./textures/numbered_grid.tga");
	m_shipTexture = new aie::Texture("./textures/ship.png");


	// Setup pool textures
	m_backgroundTexture = new aie::Texture("../bin/textures/Pool/Floor.png");
	m_tableTexture = new aie::Texture("../bin/textures/Pool/TransparentTable.png");

	PoolSetup();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_timer = 0;

	return true;
}

void Application2D::Shutdown() {
	
	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::Update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	std::cout << m_physicsScene->GetActors()[0]->GetShapeID() << std::endl;
	
	/*// Update the camera position using the arrow keys
	float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->setCameraPos(camPosX, camPosY);*/

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio,
		m_extents / m_aspectRatio,
		-1.0f, 1.0f));
	
	
	/*// demonstrate animation
	m_2dRenderer->setUVRect(int(m_timer) % 8 / 8.0f, 0, 1.f / 8, 1.f / 8);
	m_2dRenderer->drawSprite(m_texture, 200, 200, 100, 100);

	// demonstrate spinning sprite
	m_2dRenderer->setUVRect(0,0,1,1);
	m_2dRenderer->drawSprite(m_shipTexture, 600, 400, 0, 0, m_timer, 1);

	// draw a thin line
	m_2dRenderer->drawLine(300, 300, 600, 400, 2, 1);

	// draw a moving purple circle
	m_2dRenderer->setRenderColour(1, 0, 1, 1);
	m_2dRenderer->drawCircle(sin(m_timer) * 100 + 600, 150, 50);

	// draw a rotating red box
	m_2dRenderer->setRenderColour(1, 0, 0, 1);
	m_2dRenderer->drawBox(600, 500, 60, 20, m_timer);

	// draw a slightly rotated sprite with no texture, coloured yellow
	m_2dRenderer->setRenderColour(1, 1, 0, 1);
	m_2dRenderer->drawSprite(nullptr, 400, 400, 50, 50, 3.14159f * 0.25f, 1);
	
	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);*/

	// 2d tiled array background
	for (int y = 0; y < getWindowHeight() / m_backgroundTexture->getHeight() + 2; y++)
	{
		for (int x = 0; x < getWindowWidth() / m_backgroundTexture->getWidth() + 1; x++)
		{
			m_2dRenderer->drawSprite(m_backgroundTexture,
				x * m_backgroundTexture->getWidth() + m_backgroundTexture->getWidth() / 2,
				y * m_backgroundTexture->getHeight() - m_backgroundTexture->getHeight() / 2,
				m_backgroundTexture->getWidth(),
				m_backgroundTexture->getHeight(),
				0, 0);
		}
	}

	m_2dRenderer->drawSprite(m_tableTexture,
		getWindowWidth() / 2, getWindowHeight() / 2,
		getWindowWidth() - (getWindowWidth() /4), getWindowHeight() - (getWindowHeight() / 4),
		0, 0,
		0.5f, 0.5f);

	m_2dRenderer->drawSprite(m_ball1->GetBallTexture(), m_ball1->GetPosition().x, m_ball1->GetPosition().y,
		m_ball1->GetBallTexture()->getWidth(), m_ball1->GetBallTexture()->getHeight(),
		m_ball1->GetOrientation(), 0, 0.5f, 0.5f);

	// done drawing sprites
	m_2dRenderer->end();
}

void Application2D::PoolSetup()
{
    //m_physicsScene->SetGravity(glm::vec2(0));

    //Circle* ball1 = new Circle(glm::vec2(20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
    /*Circle* ball2 = new Circle(glm::vec2(0, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
    ball2->SetKinematic(true);
    ball2->SetTrigger(true);*/

    //m_physicsScene->AddActor(ball1);
    //m_physicsScene->AddActor(ball2);
    /*m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
    m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
    m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));*/

	aie::Texture* _ballTexture = new aie::Texture("../bin/textures/Pool/TransparentTable.png");
	m_ball1 = new PoolBall(
		_ballTexture, // Texture
		glm::vec2(0, 0), // position
		glm::vec2(0), // velocity
		4.0f, // mass
		4, // radius
		glm::vec4(1, 1, 1, 1), // color
		false, false); // kinematic, trigger

	Circle* whiteBall = new Circle(glm::vec2(75, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 1, 1, 1), false, false);
	m_physicsScene->AddActor(whiteBall);
	
	SetupPockets();
	SetupBalls();

	
	



	
    ///Binding the trigger function to the ball2 using Lambda expressions.
    /*ball2->TriggerEnterCallback = [=](PhysicsObject* _other)
    {
        std::cout << "Enter: " << _other << " ";
        if(_other == dynamic_cast<Box*>(_other))
        {
            std::cout << "Box" << std::endl;
        }
        if(_other == dynamic_cast<Circle*>(_other))
        {
            std::cout << "Circle" << std::endl;
        }
    };
    ball2->TriggerExitCallback = [=](PhysicsObject* _other)
    {
        std::cout << "Exit:" << _other << " ";
        if(_other == dynamic_cast<Box*>(_other))
        {
            std::cout << "Box" << std::endl;
            m_physicsScene->RemoveActor(_other);
        }
        else if(_other == dynamic_cast<Circle*>(_other))
        {
            std::cout << "Circle" << std::endl;
        }
    };*/
}

// INPUT
/*if (_input->isMouseButtonDown(0))
    {
        int xScreen, yScreen;
        _input->getMouseXY(&xScreen, &yScreen);
        glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

        aie::Gizmos::add2DCircle(worldPos, .5f, 24, glm::vec4(0, 0, 1, 1));
    }*/


void Application2D::SetupPockets()
{
	//Ends of the table
	m_physicsScene->AddActor(new Box(glm::vec2(171.5f, 0), glm::vec2(0), 0,
		4, glm::vec2(8, 69), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-171.5f, 0), glm::vec2(0), 0,
		4, glm::vec2(8, 69), glm::vec4(0, .4f, .1f, 1), true));

	// Sides of the table
	m_physicsScene->AddActor(new Box(glm::vec2(80, 89.5f), glm::vec2(0), 0,
		4, glm::vec2(70, 8), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-83.5f, 89.5f), glm::vec2(0), 0,
		4, glm::vec2(69.5, 8), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(80, -89.5f), glm::vec2(0), 0,
		4, glm::vec2(70, 8), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-83.5f, -89.5f), glm::vec2(0), 0,
		4, glm::vec2(69.5, 8), glm::vec4(0, .4f, .1f, 1), true));

	// Angled Side of the table
	// Top Left
	m_physicsScene->AddActor(new Box(glm::vec2(-166.5f, 72), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-156.5f, 85), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Top Right
	m_physicsScene->AddActor(new Box(glm::vec2(166.5f, 72), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(153.5f, 85), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Bottom Left
	m_physicsScene->AddActor(new Box(glm::vec2(-166.5f, -72), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(-156.5f, -85), glm::vec2(0), DegreeToRadian(-45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Bottom Right
	m_physicsScene->AddActor(new Box(glm::vec2(166.5f, -72), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(153.5f, -85), glm::vec2(0), DegreeToRadian(45),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));


	// Top Centre
	m_physicsScene->AddActor(new Box(glm::vec2(-12, 85), glm::vec2(0), DegreeToRadian(-25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(8, 85), glm::vec2(0), DegreeToRadian(25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	// Bottom Centre
	m_physicsScene->AddActor(new Box(glm::vec2(-12, -85), glm::vec2(0), DegreeToRadian(25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));
	m_physicsScene->AddActor(new Box(glm::vec2(8, -85), glm::vec2(0), DegreeToRadian(-25),
		4, glm::vec2(.25f, 4), glm::vec4(0, .4f, .1f, 1), true));


	//
	//// Pockets
	//Circle* topRight = new Circle(glm::vec2(220, 79), glm::vec2(0), 0, 10,
	//	glm::vec4(0, .1f, 0, 1), true, true);
	//Circle* topLeft = new Circle(glm::vec2(-217, 79), glm::vec2(0), 0, 10,
	//	glm::vec4(0, .1f, 0, 1), true, true);
	//Circle* bottomRight = new Circle(glm::vec2(220, -120), glm::vec2(0), 0, 10,
	//	glm::vec4(0, .1f, 0, 1), true, true);
	//Circle* bottomLeft = new Circle(glm::vec2(-217, -120), glm::vec2(0), 0, 10,
	//	glm::vec4(0, .1f, 0, 1), true, true);
	//Circle* topCentre = new Circle(glm::vec2(0, 85), glm::vec2(0), 0, 10,
	//	glm::vec4(0, .1f, 0, 1), true, true);
	//Circle* bottomCentre = new Circle(glm::vec2(0, -125), glm::vec2(0), 0, 10,
	//	glm::vec4(0, .1f, 0, 1), true, true);
	
	/*m_physicsScene->AddActor(topLeft);
	m_physicsScene->AddActor(topCentre);
	m_physicsScene->AddActor(topRight);
	m_physicsScene->AddActor(bottomRight);
	m_physicsScene->AddActor(bottomCentre);
	m_physicsScene->AddActor(bottomLeft);*/
	
}

void Application2D::SetupBalls()
{
	/*Plane* plane = new Plane(glm::vec2(0, 1), -50);
	m_physicsScene->AddActor(plane);

	Plane* planeD = new Plane(glm::vec2(.707f, .707f), -50);
	m_physicsScene->AddActor(planeD);#1#

	std::vector<std::string> ballLayout;
	ballLayout.push_back("0----");
	ballLayout.push_back("-0---");
	ballLayout.push_back("0-0--");
	ballLayout.push_back("-0-0-");
	ballLayout.push_back("0-0-0");
	ballLayout.push_back("-0-0-");
	ballLayout.push_back("0-0--");
	ballLayout.push_back("-0---");
	ballLayout.push_back("0----");

	SoftBody::BuildCirclesWithoutSprings(m_physicsScene, glm::vec2(-50, -49), -10.0f, 5000.0f, 5.1f, ballLayout);*/

	

	// Loop through the balls and add them to the scene in a billiards layout
	// TODO create a poolball class so we can assign properties to the balls
	
	/*for (int i = 0; i < m_balls.size(); i++)
	{
		// Set the ball's position
		m_balls[i]->SetPosition(glm::vec2(-50 + (i * 10), -49));

		// Add the ball to the scene
		m_physicsScene->AddActor(m_balls[i]);
	}*/
	
}


// Trigger function
/*void Application2D::OnBall2Check(PhysicsObject* _other)
{
	Plane* plane = dynamic_cast<Plane*>(_other);
	if (plane != nullptr)
		std::cout << "Pong!" << std::endl;
}*/

/*
glm::vec2 Application2D::ScreenToWorld(glm::vec2 _screenPos)
{
	return glm::vec2();
}
*/

glm::vec2 Application2D::ScreenToWorld(glm::vec2 _screenPos)
{
    glm::vec2 worldPos = _screenPos;

    // move the centre of the screen to (0,0)
    worldPos.x -= getWindowWidth() / 2;
    worldPos.y -= getWindowHeight() / 2;

    // scale according to our extents
    worldPos.x *= 2.0f * m_extents / getWindowWidth();
    worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

    return worldPos;
}

float Application2D::DegreeToRadian(float _degree)
 {
	return _degree * (PI / 180.f);
 }
