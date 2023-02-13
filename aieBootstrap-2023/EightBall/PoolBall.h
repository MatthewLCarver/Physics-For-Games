#pragma once

#include "Circle.h"
#include "Renderer2D.h"
#include "Texture.h"

class PoolBall : public Circle
{
public:
    PoolBall(aie::Texture* _ballTexture, glm::vec2 _position, glm::vec2 _velocity, float _mass,
        float _radius, glm::vec4 _color, bool _isKinematic, bool _isTrigger);
    ~PoolBall() override;
    //virtual void Draw(float alpha);

    aie::Texture* GetBallTexture() { return m_ballTexture; }
    
private:
    aie::Texture* m_ballTexture;
};
