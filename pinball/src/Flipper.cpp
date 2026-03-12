#include "Flipper.h"
#include <cmath>

static constexpr float DEG_TO_RAD = 3.14159265f / 180.f;

Flipper::Flipper(float x, float y, FlipperSide side)
    : m_side(side)
    , m_angularSpeed(600.f)
{
    if (m_side == FlipperSide::Left) {
        m_restAngle   = 30.f;
        m_activeAngle = -30.f;
    } else {
        m_restAngle   = 150.f;
        m_activeAngle = 210.f;
    }
    m_angle = m_restAngle;

    m_shape.setSize({WIDTH, HEIGHT});
    m_shape.setOrigin(0.f, HEIGHT / 2.f);
    m_shape.setFillColor(sf::Color(70, 130, 180));
    m_shape.setOutlineColor(sf::Color(30, 90, 140));
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition(x, y);
    m_shape.setRotation(m_angle);
}

void Flipper::handleInput(bool keyPressed, float dt)
{
    float target = keyPressed ? m_activeAngle : m_restAngle;
    float diff   = target - m_angle;

    if (std::abs(diff) < 1.f) {
        m_angle = target;
    } else {
        float step = m_angularSpeed * dt;
        if (std::abs(diff) < step) {
            m_angle = target;
        } else {
            m_angle += (diff > 0 ? step : -step);
        }
    }
    m_shape.setRotation(m_angle);
}

void Flipper::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::FloatRect Flipper::getBounds() const
{
    return m_shape.getGlobalBounds();
}

sf::Vector2f Flipper::getPosition() const
{
    return m_shape.getPosition();
}

float Flipper::getAngle() const
{
    return m_angle;
}

FlipperSide Flipper::getSide() const
{
    return m_side;
}

sf::Vector2f Flipper::getTipPosition() const
{
    float angleRad = m_angle * DEG_TO_RAD;
    sf::Vector2f origin = m_shape.getPosition();
    return {
        origin.x + WIDTH * std::cos(angleRad),
        origin.y + WIDTH * std::sin(angleRad)
    };
}
