#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <glm/vec2.hpp>

#include "PoolBall.h"


class PhysicsScene;
class PhysicsObject;

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

	PoolBall* m_ball1 = nullptr;

	void PoolSetup();
	void SetupPockets();
	void SetupBalls();
    
	void OnBall2Check(PhysicsObject* _other);
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);
	float DegreeToRadian(float _degree);
	
	const float m_extents = 250;
	const float m_aspectRatio = 16.0f / 9.0f;

	PoolBall* m_whiteBall = nullptr;
	PoolBall* m_blackBall = nullptr;
	std::vector<PoolBall*> m_solidBalls;
	std::vector<PoolBall*> m_stripedBalls;
	std::vector<Circle*> m_pockets;
};