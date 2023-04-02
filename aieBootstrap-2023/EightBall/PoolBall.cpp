#include "PoolBall.h"

#include <Gizmos.h>

/**
 * \brief Construct a new Pool Ball object
 * \param _ballTexture Set the texture of the ball
 * \param _ballType Set the type of the ball
 * \param _position Set the position of the ball
 * \param _velocity Set the velocity of the ball
 * \param _mass Set the mass of the ball
 * \param _radius Set the radius of the ball
 * \param _color Set the color of the ball
 * \param _isKinematic Set the ball to be kinematic or not
 * \param _isTrigger Set the ball to be a trigger or not
 */
PoolBall::PoolBall(aie::Texture* _ballTexture, BallType _ballType, glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius,
                   glm::vec4 _color, bool _isKinematic, bool _isTrigger): 
    Circle(_position, _velocity, _mass, _radius, _color, _isKinematic, _isTrigger)
{
    m_ballTexture = _ballTexture;
    m_ballType = _ballType;

    m_angularDrag = 1;
    m_linearDrag = 0.3f;
}

/**
 * \brief Destroy the Pool Ball object
 */
PoolBall::~PoolBall()
{
    delete m_ballTexture;
}

/**
 * \brief Debug code to draw the ball gizmos
 * \param alpha The alpha value to draw the ball at
 */
void PoolBall::Draw(float alpha)
{
    /*CalculateSmoothedPosition(alpha);
    
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius + 0.5f , 12, glm::vec4(1.f,1.f, 1.f, .5f));
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius , 12, m_color);
    aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(0.1f, 0.1f, 0.1f, 1));    */
}

/**
 * \brief Check if a point is inside the ball
 * \param _point The point to check
 * \return true If the point is inside the ball
 */
bool PoolBall::IsInside(glm::vec2 _point)
{
    return glm::distance(_point, m_position) <= m_radius;
}
