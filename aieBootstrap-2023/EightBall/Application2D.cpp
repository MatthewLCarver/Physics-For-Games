#include "Application2D.h"

#include <Demos.h>
#include <iostream>
#include <string>
#include "Box.h"
#include "Circle.h"
//#include "Plane.h""

#include <Gizmos.h>
#include "Font.h"
#include "Input.h"
#include "PhysicsObject.h"
#include "PhysicsScene.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include "PoolBall.h"

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

	int xScreen, yScreen;
	input->getMouseXY(&xScreen, &yScreen);
	m_mousePos = ScreenToWorld(glm::vec2(xScreen, yScreen));
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

	HandleGameplay(input);

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
	

	//m_2dRenderer->setRenderColour(1,1,1,0.5f);
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

	// Draw box Cue stick
	//m_2dRenderer->drawBox(WorldToScreen(m_mousePos).x, WorldToScreen(m_mousePos).y, 500, 10, GetAngleBetweenPoints(m_whiteBall->GetPosition(), m_mousePos));
	m_2dRenderer->drawLine(WorldToScreen(m_mousePos).x, WorldToScreen(m_mousePos).y, WorldToScreen(m_whiteBall->GetPosition()).x, WorldToScreen(m_whiteBall->GetPosition()).y, 10, 1);
	
	
	// 2d tiled array background
	for (int y = 0; y < (int)getWindowHeight() / (int)m_backgroundTexture->getHeight() + 2; y++)
	{
		for (int x = 0; x < (int)getWindowWidth() / (int)m_backgroundTexture->getWidth() + 1; x++)
		{
			m_2dRenderer->drawSprite(m_backgroundTexture,
				(float)(x * m_backgroundTexture->getWidth()) + (float)m_backgroundTexture->getWidth() / 2,
				(float)(y * m_backgroundTexture->getHeight()) - (float)m_backgroundTexture->getHeight() / 2,
				(float)m_backgroundTexture->getWidth(),
				(float)m_backgroundTexture->getHeight(),
				0, 0);
		}
	}

	m_2dRenderer->drawSprite(m_tableTexture,
		(float)getWindowWidth() / 2, (float)getWindowHeight() / 2,
		(float)(getWindowWidth()) - ((float)getWindowWidth() /4), (float)(getWindowHeight()) - ((float)getWindowHeight() / 4),
		0, 0,
		0.5f, 0.5f);

	/*m_2dRenderer->drawSprite(
		m_ball1->GetBallTexture(), 
		m_ball1->GetPosition().x, m_ball1->GetPosition().y,
		m_ball1->GetRadius() * 2, m_ball1->GetRadius() * 2,
		m_ball1->GetOrientation(), 0, 0.5f, 0.5f*/
		
		/*m_2dRenderer->drawSprite(
		m_blackBall->GetBallTexture(), 
		m_blackBall->GetPosition().x, m_blackBall->GetPosition().y,
		m_blackBall->GetRadius() * 2, m_blackBall->GetRadius() * 2,
		m_blackBall->GetOrientation(), 0, 0.5f, 0.5f
		);*/
	for(int i = 0; i < m_solidBalls.size(); i++)
	{
		m_2dRenderer->drawSprite(
			m_solidBalls[i]->GetBallTexture(),
			WorldToScreen(m_solidBalls[i]->GetPosition()).x,
			WorldToScreen(m_solidBalls[i]->GetPosition()).y,
			m_solidBalls[i]->GetRadius() * 8, m_solidBalls[i]->GetRadius() * 8,
			m_solidBalls[i]->GetOrientation(), 0, 0.5f, 0.5f
		);
	}
	for(int i = 0; i < m_stripedBalls.size(); i++)
	{
		m_2dRenderer->drawSprite(
			m_stripedBalls[i]->GetBallTexture(), 
			WorldToScreen(m_stripedBalls[i]->GetPosition()).x,
			WorldToScreen(m_stripedBalls[i]->GetPosition()).y,
			m_stripedBalls[i]->GetRadius() * 8, m_stripedBalls[i]->GetRadius() * 8,
			m_stripedBalls[i]->GetOrientation(), 0, 0.5f, 0.5f
		);
	}
		
	
	
	m_2dRenderer->drawSprite(
		m_whiteBall->GetBallTexture(),
		WorldToScreen(m_whiteBall->GetPosition()).x,
		WorldToScreen(m_whiteBall->GetPosition()).y,
		m_whiteBall->GetRadius() * 8, m_whiteBall->GetRadius() * 8,
		m_whiteBall->GetOrientation(), 0, 0.5f, 0.5f
	);
	m_2dRenderer->drawSprite(
		m_blackBall->GetBallTexture(),
		WorldToScreen(m_blackBall->GetPosition()).x,
		WorldToScreen(m_blackBall->GetPosition()).y,
		m_blackBall->GetRadius() * 8, m_blackBall->GetRadius() * 8,
		m_blackBall->GetOrientation(), 0, 0.5f, 0.5f
	);

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

	
	SetupTableAndPockets();
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



void Application2D::SetupTableAndPockets()
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


	
	// Pockets
	Circle* topRight = new Circle(glm::vec2(170, 88), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* topLeft = new Circle(glm::vec2(-172, 88), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* bottomRight = new Circle(glm::vec2(170, -88), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* bottomLeft = new Circle(glm::vec2(-172, -88), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* topCentre = new Circle(glm::vec2(-2, 92.5f), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	Circle* bottomCentre = new Circle(glm::vec2(-2, -92.5f), glm::vec2(0), 0, 6,
		glm::vec4(0, .1f, 0, 1), true, true);
	
	m_physicsScene->AddActor(topLeft);
	m_physicsScene->AddActor(topCentre);
	m_physicsScene->AddActor(topRight);
	m_physicsScene->AddActor(bottomRight);
	m_physicsScene->AddActor(bottomCentre);
	m_physicsScene->AddActor(bottomLeft);

	m_pockets.push_back(topLeft);
	m_pockets.push_back(topCentre);
	m_pockets.push_back(topRight);
	m_pockets.push_back(bottomRight);
	m_pockets.push_back(bottomCentre);
	m_pockets.push_back(bottomLeft);

	for( int i = 0; i < m_pockets.size(); i++)
	{
		m_pockets[i]->TriggerEnterCallback = [=] (PhysicsObject* _other)
		{
			PoolBall* ball = dynamic_cast<PoolBall*>(_other);
			OnPoolBallSunkTrigger(ball);
		};
	}
}

void Application2D::AddToBallList()
{
	// Loops through all the balls in play and add them to a single list for velocity checking
	m_allBalls.push_back(m_whiteBall);
	m_allBalls.push_back(m_blackBall);
	for (int i = 0; i < 7; i++)
	{
		m_allBalls.push_back(m_solidBalls[i]);
		m_allBalls.push_back(m_stripedBalls[i]);
	}
}

void Application2D::SetupBalls()
{
	// Setup the ball textures
	std::vector<aie::Texture*> m_ballTex = std::vector<aie::Texture*>(16); 
	for( int i = 1; i < 17; i++)
	{
		m_ballTex[i-1] = new aie::Texture(("../bin/textures/Pool/ball_" + std::to_string(i) + ".png").c_str());
	}

	m_whiteBall = new PoolBall(
		m_ballTex[15], // Texture
		WHITE, // Ball Type
		glm::vec2(100, 0), // position
		glm::vec2(0), // velocity
		m_poolBallMass, // mass
		m_poolBallRadius, // radius
		glm::vec4(1, 1, 1, 1), // color
		false, false); // kinematic, trigger
	m_physicsScene->AddActor(m_whiteBall);

	// Place the balls into a triangle on the table
	int ballsPlaced = 0;
	float heightAdjuster = 0;
	float poolDiameter = m_poolBallRadius * 2;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 1 + i; j++)
		{
			int mid = (1 + i) / 2;
			if(mid == j)
			{
				heightAdjuster = 0;
			}
			else if(mid > j)
			{
				heightAdjuster = m_poolBallRadius + (mid - j) * m_poolBallRadius;
			}
			else
			{
				heightAdjuster = -m_poolBallRadius - (j - mid) * m_poolBallRadius;
			}
			
			if(ballsPlaced == 4)
			{
				// push back the 8 ball in next position
				m_blackBall = new PoolBall(
					m_ballTex[7], // Texture
					BLACK, // Ball Type
						glm::vec2( // position
						-100 - (i * poolDiameter),
						0 + heightAdjuster),
					glm::vec2(0), // velocity
					m_poolBallMass, // mass
					m_poolBallRadius, // radius
					glm::vec4(1, 1, 1, 1), // color
					false, false); // kinematic, trigger
				m_physicsScene->AddActor(m_blackBall);
				ballsPlaced++;
				continue;
			}
			if(i % 2 != 0)
			{
				heightAdjuster -= m_poolBallRadius;
			}
			if (ballsPlaced % 2 != 1)
			{
				m_solidBalls.push_back(new PoolBall(
					m_ballTex[m_solidBalls.size()], // Texture
					SOLID, // Ball Type
						glm::vec2( // position
						-100 - (i * poolDiameter),
						0 + heightAdjuster),
					glm::vec2(0), // velocity
					m_poolBallMass, // mass
					m_poolBallRadius, // radius
					glm::vec4(1, 1, 1, 1), // color
					false, false)); // kinematic, trigger
				m_physicsScene->AddActor(m_solidBalls[m_solidBalls.size()-1]);
			}
			else
			{
				m_stripedBalls.push_back(new PoolBall(
					m_ballTex[m_stripedBalls.size() + 8], // Texture
					STRIPED, // Ball Type
						glm::vec2( // position
						-100 - (i * poolDiameter),
						0 + heightAdjuster),
					glm::vec2(0), // velocity
					m_poolBallMass, // mass
					m_poolBallRadius, // radius
					glm::vec4(1, 1, 1, 1), // color
					false, false)); // kinematic, trigger
				m_physicsScene->AddActor(m_stripedBalls[m_stripedBalls.size()-1]);
			}
			
			//m_physicsScene->AddActor(m_solidBalls[i]);
			ballsPlaced++;
		}
	}

	// Add all the balls to a single list
	AddToBallList();
}

//Trigger function
void Application2D::OnPoolBallSunkTrigger(PoolBall* _ball)
{
	switch((int)_ball->GetBallType())
	{
	case (int)WHITE:
		std::cout << "WHITE BALL SUNK" << std::endl;
		break;
		
	case (int)BLACK:
		std::cout << "BLACK BALL SUNK" << std::endl;
		break;
		
	case (int)SOLID:
		std::cout << "SOLID BALL SUNK" << std::endl;
		break;
		
	case (int)STRIPED:
		std::cout << "STRIPED BALL SUNK" << std::endl;
		break;

	default:
		std::cout << "BROKEN BALL SUNK" << std::endl;
		break;
	}
}

bool Application2D::CheckVelocities()
{
	for(int i = 0; i < m_allBalls.size(); i++)
	{
		if (m_allBalls[i]->GetVelocity().x > 0.1f || m_allBalls[i]->GetVelocity().y > 0.1f)
		{
			return true;
		}
	}
	return false;
}

void Application2D::CheckPocketTriggers()
{
	// loop through all the pockets in the m_pockets vector
	for(int i = 0; i < m_pockets.size(); i++)
	{
		// loop through all the balls in the m_allBalls vector
		for(int j = 0; j < m_allBalls.size(); j++)
		{
			// if the ball is in the pocket
			if(m_pockets[i]->IsInside(m_allBalls[j]->GetPosition()))
			{
				
			}
		}
	}
}

void Application2D::HandleGameplay(aie::Input* _input)
{
	if(CheckVelocities())
	{
		m_isShooting = false;
		return;
	}

	CheckPocketTriggers();
	
	// INPUT
	if (_input->isMouseButtonDown(0))
	{
		m_isShooting = true;
		//glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

		aie::Gizmos::add2DCircle(m_mousePos, .5f, 24, glm::vec4(0, 0, 1, 1));

		//m_whiteBall->ApplyForce(glm::vec2(-100, 0), glm::vec2(0));
		
	}
}

glm::vec2 Application2D::ScreenToWorld(glm::vec2 _screenPos)
{
    glm::vec2 worldPos = _screenPos;

    // move the centre of the screen to (0,0)
    worldPos.x -= (float)getWindowWidth() / 2;
    worldPos.y -= (float)getWindowHeight() / 2;

    // scale according to our extents
    worldPos.x *= 2.0f * m_extents / getWindowWidth();
    worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

    return worldPos;
}

glm::vec2 Application2D::WorldToScreen(glm::vec2 _worldPos)
{
	glm::vec2 screenPos = _worldPos;
	screenPos.x *= getWindowWidth() / (2.0f * m_extents);
	screenPos.y *= (m_aspectRatio * getWindowHeight()) / (2.0f * m_extents);
	
	screenPos.x += (float)getWindowWidth() / 2;
	screenPos.y += (float)getWindowHeight() / 2;

	return screenPos;
}

float Application2D::DegreeToRadian(float _degree)
 {
	return _degree * (float)(PI / (double)180.f);
 }

float Application2D::RadianToDegree(float _radian)
{
	return _radian * (float)(180.f / (double)PI);
}

float Application2D::GetAngleBetweenPoints(glm::vec2 _point1, glm::vec2 _point2)
{
	return atan2(_point2.y - _point1.y, _point2.x - _point1.x);
}