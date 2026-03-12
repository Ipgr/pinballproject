#pragma once
#include <SFML/Graphics.hpp>

enum class FlipperSide { Left, Right };

class Flipper {
public:
    Flipper(float x, float y, FlipperSide side);

    void handleInput(bool keyPressed, float dt);
    void draw(sf::RenderWindow& window) const;

    // Returns the transformed shape for collision
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    float getAngle() const;
    FlipperSide getSide() const;

    // Returns the tip of the flipper for ball-hit impulse
    sf::Vector2f getTipPosition() const;

private:
    sf::RectangleShape m_shape;
    FlipperSide m_side;

    float m_angle;        // current rotation angle (degrees)
    float m_restAngle;    // resting angle
    float m_activeAngle;  // activated angle
    float m_angularSpeed; // degrees per second

    static constexpr float WIDTH  = 80.f;
    static constexpr float HEIGHT = 16.f;
};
