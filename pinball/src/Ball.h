#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float x, float y, float radius = 12.f);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    void reset(float x, float y);

    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    sf::Vector2f getVelocity() const;
    void setVelocity(float vx, float vy);
    void addVelocity(float vx, float vy);
    float getRadius() const;

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
    float m_radius;

    static constexpr float GRAVITY = 600.f;
    static constexpr float MAX_SPEED = 1000.f;
};
