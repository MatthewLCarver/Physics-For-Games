#pragma once

#include <Input.h>

#include "Application.h"
#include "Renderer2D.h"
#include <glm/vec2.hpp>
#include <vector>



class PoolBall;
class PhysicsScene;
class PhysicsObject;
class Circle;

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool Startup();
	virtual void Shutdown();

	
	virtual void Update(float deltaTime);
	virtual void draw();


protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;
	aie::Font*			m_font;

	float m_timer;

	float GetExtents() { return m_extents; }
	float GetAspect() { return m_aspectRatio; }

private:
	PhysicsScene* m_physicsScene;

	aie::Texture* m_backgroundTexture = nullptr;
	aie::Texture* m_tableTexture = nullptr;

	// Init Functions
	void PoolSetup();
	void SetupTableAndPockets();
	void AddToBallList();
	void SetupBalls();

	// Update Functions
	bool CheckVelocities();
	void CheckPocketTriggers();
    void HandleGameplay(aie::Input* _input);
	void OnPoolBallSunkTrigger(PoolBall* _other);
	
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);
	glm::vec2 WorldToScreen(glm::vec2 _worldPos);
	float DegreeToRadian(float _degree);
	float RadianToDegree(float _radian);
	float GetAngleBetweenPoints(glm::vec2 _point1, glm::vec2 _point2);

	const float m_extents = 250;
	const float m_aspectRatio = 16.0f / 9.0f;

	glm::vec2 m_mousePos = glm::vec2(0);

	float m_poolBallRadius = 5;
	float m_poolBallMass = 3;

	PoolBall* m_whiteBall = nullptr;
	PoolBall* m_blackBall = nullptr;
	std::vector<PoolBall*> m_solidBalls;
	std::vector<PoolBall*> m_stripedBalls;
	std::vector<PoolBall*> m_allBalls;
	
	std::vector<Circle*> m_pockets;
	

	// Bools
	bool m_isShooting = false;
};