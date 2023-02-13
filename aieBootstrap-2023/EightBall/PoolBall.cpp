#include "PoolBall.h"


PoolBall::PoolBall(aie::Texture* _ballTexture, glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius,
    glm::vec4 _color, bool _isKinematic, bool _isTrigger): Circle(_position, _velocity, _mass, _radius, _color, _isKinematic, _isTrigger)
{
    m_ballTexture = _ballTexture;
}

PoolBall::~PoolBall()
{
    delete m_ballTexture;
}

/*void PoolBall::Draw(float alpha)
{
    
}*/
