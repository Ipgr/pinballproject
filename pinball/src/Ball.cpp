#include "Ball.h"
#include <cmath>
#include <algorithm>

Ball::Ball(float x, float y, float radius)
    : m_radius(radius)
    , m_velocity(0.f, 0.f)
{
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setFillColor(sf::Color(220, 220, 220));
    m_shape.setOutlineColor(sf::Color(160, 160, 160));
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition(x, y);
}

void Ball::update(float dt)
{
    // Apply gravity
    m_velocity.y += GRAVITY * dt;

    // Clamp speed
    float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
    if (speed > MAX_SPEED) {
        m_velocity *= MAX_SPEED / speed;
    }

    m_shape.move(m_velocity * dt);
}

void Ball::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

void Ball::reset(float x, float y)
{
    m_shape.setPosition(x, y);
    m_velocity = {0.f, 0.f};
}

sf::Vector2f Ball::getPosition() const
{
    return m_shape.getPosition();
}

void Ball::setPosition(float x, float y)
{
    m_shape.setPosition(x, y);
}

sf::Vector2f Ball::getVelocity() const
{
    return m_velocity;
}

void Ball::setVelocity(float vx, float vy)
{
    m_velocity = {vx, vy};
}

void Ball::addVelocity(float vx, float vy)
{
    m_velocity.x += vx;
    m_velocity.y += vy;
}

float Ball::getRadius() const
{
    return m_radius;
}
